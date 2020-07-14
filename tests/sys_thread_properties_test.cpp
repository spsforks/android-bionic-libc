TEST(thread_properties_test, iterate_dts) {
#if defined(__BIONIC__)
  const char expected_out[] =
      "got test_static_tls_bounds\niterate_cb i = 0\ndone_iterate_dynamic_tls\n";
  std::string helper = GetTestLibRoot() + "tls_properties_helper/tls_properties_helper";
  chmod(helper.c_str(), 0755);

  ExecTestHelper eth;
  eth.SetArgs({helper.c_str(), nullptr});
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, expected_out);
#endif
}

TEST(thread_properties_test, thread_exit_cb) {
#if defined(__BIONIC__)
  // tests/libs/thread_exit_cb_helper.cpp
  const char expected_out[] = "exit_cb_1 called exit_cb_2 called exit_cb_3 called";
  std::string helper = GetTestLibRoot() + "thread_exit_cb_helper/thread_exit_cb_helper";
  chmod(helper.c_str(), 0755);

  ExecTestHelper eth;
  eth.SetArgs({helper.c_str(), nullptr});
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, expected_out);

#endif
}
