#pragma once

#include <defines.h>

typedef struct Application {
    void* internal_state;
    void* app_state;
} Application;

extern b8 application_initialize(Application* out_application); 

SKLAPI b8 application_create(Application* app_inst);