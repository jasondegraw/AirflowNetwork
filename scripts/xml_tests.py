# Copyright (c) 2019, Jason W. DeGraw
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import unittest
import warnings
import glob
import os
from lxml import etree
from io import BytesIO, StringIO

schema_file = '../input/AirflowNetwork.xsd'
tree = etree.parse(schema_file)
schema = etree.XMLSchema(tree)


def validate(filename, schema, instance):
    try:
        schema.assertValid(instance)
    except etree.DocumentInvalid as exc:
        return filename + ': ' + str(exc)
    return ''


pass_files = []
fail_files = []
for file in glob.glob('../input/*.xml'):
    if file.endswith('-fail.xml'):
        fail_files.append(file)
    else:
        pass_files.append(file)


failure_table = {'example1-badref-fail.xml' : "No match found for key-sequence ['three-four'] of keyref '{http://github.com/jasondegraw/AirflowNetwork}NodeKeyRef'., line 70"
                }


class ValidateExampleFiles(unittest.TestCase):
    def setUp(self):
        pass

    def test_pass_files(self):
        for file in pass_files:
            warnings.simplefilter("ignore")
            data = etree.parse(file)
            self.assertEqual(validate(file, schema, data), '')

    def test_fail_files(self):
        for file in fail_files:
            warnings.simplefilter("ignore")
            data = etree.parse(file)
            result = validate(file, schema, data)
            self.assertTrue(os.path.basename(file) in failure_table, msg=file)
            self.assertTrue(failure_table[os.path.basename(file)] in result)


if __name__ == '__main__':
    unittest.main()

