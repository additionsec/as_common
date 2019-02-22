#!/usr/bin/env python

import addsec_cti_pb2
import time
import urllib2

import ssl
if hasattr(ssl, '_create_unverified_context'):
	ssl._create_default_https_context = ssl._create_unverified_context

def test_post( url ):

	report = addsec_cti_pb2.Report()
	report.organizationId = "bb54cfac59e73d9dae01b84bd476bbadde7d8747".decode('hex')
	report.systemId = "\xbb\xbb\xbb\xbb"
	report.systemIdSecondary = "mysysid"
	report.systemType = 2
	report.applicationId = "com.example.app"
	report.userId = "\xcc\xcc\xcc\xcc"
	report.userIdSecondary = "myuser@example.com"

	ob = report.sightings.add()
	ob.sightingType = 2
	ob.timestamp = int(time.time())
	ob.confidence = 2
	ob.impact = 3
	ob.testId = 1001
	ob.testSubId = 2002

	dat = ob.datas.add()
	dat.dataType = 8
	dat.data = "phone"

	dat = ob.datas.add()
	dat.dataType = 10
	dat.data = "/dev/null"

	dat = ob.datas.add()
	dat.dataType = 19
	dat.num = 42

	ob = report.sightings.add()
	ob.sightingType = 4
	ob.timestamp = int(time.time())
	ob.confidence = 0
	ob.impact = 0
	ob.testId = 1

	dat = ob.datas.add()
	dat.dataType = 18
	dat.data = "custom=da|ta \x00 & \"stuff\" back\\ 'and'\tstuff"

	dat = ob.datas.add()
	dat.dataType = 26
	dat.data = str("\xB5\x7B\x2A\x78") # 2016050101 in little endian order

	dat = ob.datas.add()
	dat.dataType = 22
	dat.data = str("\xBB\x01") # 443 in little endian order


	payload = report.SerializeToString()

	"""
	with open("/tmp/cti.bin","w") as f:
		f.write(payload)
	"""

	req = urllib2.Request(url, payload)
	req.add_header('Content-Type','application/octet-stream')
	f = urllib2.urlopen(req)
	#f = urllib.urlopen( url, payload )
	print f.getcode()


if __name__ == "__main__":
	import sys
	url = sys.argv[1]
	test_post( url )
