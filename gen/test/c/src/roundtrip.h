#ifndef MX_CORERT_ROUNDTRIP_H
#define MX_CORERT_ROUNDTRIP_H

typedef struct {
    int ok;
    int skipped; /* document declares a newer MusicXML than this target */
    char message[1024];
} RoundtripResult;

RoundtripResult run_core_roundtrip(const char *abs_input_path);

#endif
