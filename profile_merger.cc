// Copyright 2014 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Merge the .afdo files.

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/common.h"
#include "base/logging.h"
#include "gcov.h"
#include "profile_reader.h"
#include "profile_writer.h"
#include "symbol_map.h"
#include "module_grouper.h"

DEFINE_string(output_file, "fbdata.afdo",
              "Output file name");

int main(int argc, char **argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  autofdo::SymbolMap symbol_map;
  autofdo::ModuleMap module_map;

  if (argc < 3) {
    LOG(FATAL) << "Please at least specify two files to merge";
  }

  // TODO(dehao): merge profile reader/writer into a single class
  for (int i = 1; i < argc; i++) {
    autofdo::AutoFDOProfileReader reader(
        &symbol_map, &module_map);
    reader.ReadFromFile(argv[i]);
  }

  autofdo::AutoFDOProfileWriter writer(symbol_map,
      module_map, FLAGS_gcov_version);
  if (!writer.WriteToFile(FLAGS_output_file)) {
    LOG(FATAL) << "Error writing to " << FLAGS_output_file;
  }
  return 0;
}
