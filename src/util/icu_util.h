// Copyright [2017] <Malinovsky Rodion>

#pragma once

#include <memory>
#include <vector>

#include <unicode/unistr.h>
#include <unicode/sortkey.h>
#include <unicode/utypes.h>

namespace cppecho {
namespace util {
namespace icu_util {

void DoSort() {

  const std::vector<UnicodeString> strings = {
    "Quick",
    "fox",
    "Moving",
    "trucks",
    "riddle"
  };

  std::vector<CollationKey*> keys(strings.size(), nullptr);

  UErrorCode status = U_ZERO_ERROR;
  std::unique_ptr<Collator> collator(Collator::createInstance(Locale("en_US"), status));
  if(U_SUCCESS(status)) {
    for(uint32_t i=0; i< strings.size(); ++i) {
      // keys[i] = collator->getCollationKey(strings[i], -1);
    }
    // qsort(keys, strings.size(), sizeof(CollationKey), compareKeys);
    // delete[] keys;
  }

}

}  // namespace icu_util
}  // namespace util
}  // namespace cppecho
