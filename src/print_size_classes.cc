// Simple program to print TCMalloc size class information
// Compile: g++ -o print_size_classes print_size_classes.cc -ltcmalloc
// Or link with your TCMalloc build

#include <stdio.h>
#include "gperftools/malloc_extension.h"

int main() {
  size_t num_size_classes = 0;
  size_t max_size_classes = 0;
  size_t max_size = 0;
  size_t current_allocated = 0;
  
  // Get size class information
  if (MallocExtension::instance()->GetNumericProperty(
        "tcmalloc.num_size_classes", &num_size_classes)) {
    printf("Number of active size classes: %zu\n", num_size_classes);
  } else {
    printf("Failed to get num_size_classes\n");
  }
  
  if (MallocExtension::instance()->GetNumericProperty(
        "tcmalloc.max_size_classes", &max_size_classes)) {
    printf("Maximum size classes (kClassSizesMax): %zu\n", max_size_classes);
  }
  
  if (MallocExtension::instance()->GetNumericProperty(
        "tcmalloc.max_size", &max_size)) {
    printf("Maximum size for size classes (kMaxSize): %zu bytes (%zu KB)\n", 
           max_size, max_size / 1024);
  }
  
  if (MallocExtension::instance()->GetNumericProperty(
        "generic.current_allocated_bytes", &current_allocated)) {
    printf("Current allocated bytes: %zu\n", current_allocated);
  }
  
  // Print utilization
  if (num_size_classes > 0 && max_size_classes > 0) {
    double utilization = (double)num_size_classes / max_size_classes * 100.0;
    printf("\nSize class array utilization: %.1f%% (%zu / %zu)\n",
           utilization, num_size_classes, max_size_classes);
    
    if (num_size_classes >= max_size_classes * 0.9) {
      printf("WARNING: Size class array is nearly full!\n");
      printf("Consider increasing kClassSizesMax if you plan to reduce kMaxSize.\n");
    }
  }
  
  // Do a simple allocation to ensure TCMalloc is initialized
  void* p = malloc(1024);
  free(p);
  
  return 0;
}
