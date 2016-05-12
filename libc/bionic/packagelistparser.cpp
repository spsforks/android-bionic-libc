/*
 * Copyright 2015, Intel Corporation
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Written by William Roberts <william.c.roberts@intel.com>
 *
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/limits.h>

#include <private/packagelistparser.h>

static size_t get_gid_cnt(const char* gids) {
  if (*gids == '\0') {
    return 0;
  }

  if (strcmp(gids, "none") == 0) {
    return 0;
  }

  size_t cnt;
  for (cnt = 1; gids[cnt]; gids[cnt] == ',' ? cnt++ : *gids++)
    ;

  return cnt;
}

static bool parse_gids(char* gids, gid_t* gid_list, size_t* cnt) {
  size_t cmp = 0;
  char* token;

  while ((token = strsep(&gids, ",\r\n"))) {

    if (cmp > *cnt) {
      return false;
    }

    char* endptr;
    gid_t gid = strtoul(token, &endptr, 10);
    if (*endptr != '\0') {
      return false;
    }

    // if unsigned long is greater than size of gid_t,
    // prevent a truncation based roll-over
    if (gid > GID_MAX) {
      return false;
    }

    gid_list[cmp++] = gid;
  }
  return true;
}

bool packagelist_parse(pfn_on_package callback, void* userdata) {
  bool rc = false;

  FILE* fp = fopen(PACKAGES_LIST_FILE, "re");
  if (fp == NULL) {
    return rc;
  }

  ssize_t bytesread;
  char* buf = NULL;
  size_t buflen = 0;
  pkg_info* info = NULL;

  while ((bytesread = getline(&buf, &buflen, fp)) > 0) {

    info = static_cast<pkg_info*>(calloc(1, sizeof(*info)));
    if (info == NULL) {
      goto err;
    }

    char* next = buf;
    char* cur = strsep(&next, " \t\r\n");
    if (cur == NULL) {
      goto err;
    }

    info->name = strdup(cur);
    if (info->name == NULL) {
      goto err;
    }

    cur = strsep(&next, " \t\r\n");
    if (cur == NULL) {
      goto err;
    }

    char* endptr;
    unsigned long tmp = strtoul(cur, &endptr, 10);
    if (*endptr != '\0') {
      goto err;
    }

    // if unsigned long is greater than size of uid_t,
    // prevent a truncation based roll-over
    if (tmp > UID_MAX) {
      goto err;
    }

    info->uid = static_cast<uid_t>(tmp);

    cur = strsep(&next, " \t\r\n");
    if (cur == NULL) {
      goto err;
    }

    tmp = strtoul(cur, &endptr, 10);
    if (*endptr != '\0') {
      goto err;
    }

    // should be a valid boolean of 1 or 0
    if ((tmp != 0) && (tmp != 1)) {
      goto err;
    }

    info->debuggable = tmp != 0;

    cur = strsep(&next, " \t\r\n");
    if (cur == NULL) {
      goto err;
    }

    info->data_dir = strdup(cur);
    if (info->data_dir == NULL) {
      goto err;
    }

    cur = strsep(&next, " \t\r\n");
    if (cur == NULL) {
      goto err;
    }

    info->seinfo = strdup(cur);
    if (info->seinfo == NULL) {
      goto err;
    }

    cur = strsep(&next, " \t\r\n");
    if (cur == NULL) {
      goto err;
    }

    // Parse the gid list, could be in the form of none, single gid or list:
    //  none
    //  gid
    //  gid, gid ...
    info->gids.cnt = get_gid_cnt(cur);
    if (info->gids.cnt > 0) {

      info->gids.gids = static_cast<gid_t*>(calloc(info->gids.cnt, sizeof(gid_t)));
      if (info->gids.gids == NULL) {
        goto err;
      }

      rc = parse_gids(cur, info->gids.gids, &info->gids.cnt);
      if (rc == 0) {
        goto err;
      }
    }

    rc = callback(info, userdata);
    if (rc == false) {
      // We do not log this as this can be intentional from callback to abort
      // processing. We go to out to not free the (already freed) package info.
      break;
    }
  }

  rc = true;

out:
  free(buf);
  fclose(fp);
  return rc;

err:
  rc = false;
  packagelist_free(info);
  goto out;
}

void packagelist_free(pkg_info* info) {
  if (info == NULL) {
    return;
  }
  free(info->name);
  free(info->data_dir);
  free(info->seinfo);
  free(info->gids.gids);
  free(info);
}
