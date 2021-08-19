# 
#     Copyright 2021 CSEM SA
# 
#     Licensed under the Apache License, Version 2.0 (the "License");
#     you may not use this file except in compliance with the License.
#     You may obtain a copy of the License at
# 
#         http://www.apache.org/licenses/LICENSE-2.0
# 
#     Unless required by applicable law or agreed to in writing, software
#     distributed under the License is distributed on an "AS IS" BASIS,
#     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#     See the License for the specific language governing permissions and
#     limitations under the License.
# 

import os
from config import *

test_dir = os.path.dirname(os.path.abspath(__file__))
image_build_dir = test_dir+"/../../../hello_world/build"

image_valid_primary = "image_valid"
image_invalid_signature_primary = "image_wrong_sign"
image_corrupted_primary = "image_corrupted"
image_invalid_encryption_primary = "image_wrong_enc"
image_valid_secondary = image_valid_primary + "_enc"
image_invalid_signature_secondary = image_invalid_signature_primary + "_enc"
image_invalid_encryption_secondary = image_invalid_encryption_primary + "_enc"