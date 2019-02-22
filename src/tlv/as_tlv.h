#ifndef _AS_TLV_H_
#define _AS_TLV_H_

// Copyright 2019 J Forristal LLC
// Copyright 2016 Addition Security Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdint.h>

#define ASTLV_CALLBACK(nom)  void(*(nom))(uint8_t tag, uint16_t len, uint8_t* data, void* state)
#define ASTLV_CALLBACK_DEF(nom)  void (nom)(uint8_t tag, uint16_t len, uint8_t* data, void* state)

uint8_t ASTLV_Parse( uint8_t *data, uint16_t data_len, ASTLV_CALLBACK(callback), void *state );


#endif
