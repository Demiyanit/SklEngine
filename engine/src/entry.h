#pragma once

#include <defines.h>
#include <core/application.h>
#include <stdio.h>

int main() {
    Application* app_inst = NULL;

    if(!application_initialize(app_inst)) {
        printf("Failed to initialize application! Shutting down...\n");
        return -1;
    }

    if(!application_create(app_inst)) {
        printf("Failed to create application! Shutting down...");
        return -2;
    }
}