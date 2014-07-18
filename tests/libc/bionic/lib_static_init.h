#ifndef _lib_static_init_h
#define _lib_static_init_h

class Foos {
private:
    int         value;
    static int  counter;
public:
    virtual int getValue();
    Foos();
    virtual ~Foos();
};

extern Foos  theFoo;

#endif /* _lib_static_init_h */
