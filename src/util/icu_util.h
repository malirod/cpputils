// Copyright [2017] <Malinovsky Rodion>

#pragma once

namespace cppecho {
namespace util {
namespace icu_util {

CollationKey* keys[MAX_LIST_LENGTH];
UErrorCode status = U_ZERO_ERROR;
Collator* coll = Collator::createInstance(Locale("en_US"), status);
uint32_t i;
if (U_SUCCESS(status)) {
  for (i = 0; i < listSize; i++) {
    keys[i] = coll->getCollationKey(s[i], -1);
  }
  qsort(keys, MAX_LIST_LENGTH, sizeof(CollationKey), compareKeys);
  delete[] keys;
  delete coll;
}

}  // namespace icu_util
}  // namespace util
}  // namespace cppecho
