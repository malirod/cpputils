BEGIN {
  # Warnings from he followwing list of files will be ignored.
  # suppressed["relative/path/to/file1"]
  # suppressed["relative/path/to/file2"]
  # suppressed["src/util/logger.h"]
}

# Suppress warnings for the auto-generated files
/.*\.(pb|proxy)\.(cc|cpp|h|hpp) should (add|remove) these lines:/, /^$/ {
  next
}

# Suppress warnings for the explicitly specified files
/.* should (add|remove) these lines:/ {
  can_print = 1
  for (path in suppressed) {
    if (index($0, path)) {
      can_print = 0
      break
    }
  }
}
/^$/ {
  if (can_print) print
  can_print = 0
}
{ if (can_print) print }
