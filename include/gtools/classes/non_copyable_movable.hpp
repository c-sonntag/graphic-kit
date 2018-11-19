#pragma once

#include <bits/move.h>

#define gtools_classes_non_copyable_movable(classname)  \
  /* Enable move.  */                                   \
  classname( classname && ) = default;                  \
  /* Disable copy from lvalue. */                       \
  classname( const classname & ) = delete;              \
  classname & operator=( const classname & ) = delete;

