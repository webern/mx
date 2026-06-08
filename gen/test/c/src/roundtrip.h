#ifndef MX_CORERT_ROUNDTRIP_H
#define MX_CORERT_ROUNDTRIP_H

typedef struct {
    int ok;
    char message[1024];
} RoundtripResult;

RoundtripResult run_core_roundtrip(const char *abs_input_path);

#endif
