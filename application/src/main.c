#include <stdlib.h>
#include <entry.h>

b8 application_initialize(Application* out_application) {
    out_application = malloc(sizeof(Application));
    printf("STARTUP\n");
    return true;
}