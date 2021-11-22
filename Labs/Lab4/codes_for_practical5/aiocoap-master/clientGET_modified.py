#!/usr/bin/env python3

# This file is part of the Python aiocoap library project.
#
# Copyright (c) 2012-2014 Maciej Wasilak <http://sixpinetrees.blogspot.com/>,
#               2013-2014 Christian Amsüss <c.amsuess@energyharvesting.at>
#
# aiocoap is free software, this file is published under the MIT license as
# described in the accompanying LICENSE file.

import logging
import asyncio
import time
import sys

from aiocoap import *

logging.basicConfig(level=logging.INFO)

@asyncio.coroutine
def main():
    x=1
    reading_interval = 1 #The time interval between each GET request in second
    reading_num = 10 #How many RSSI samples you try to get? 

    while x<reading_num:
        protocol = yield from Context.create_client_context()

        request = Message(code=GET)
        #Configure the IP address of the CoAP server here
        #Also, you may need to change the URL depending on the implementation of your CoAP server
        #request.set_request_uri('coap://[aaaa::212:4b00:a56:1200]:5683/net/parent/RSSI')
		request.set_request_uri('coap://[aaaa::212:4b00:1205:2269]:5683/rssi')

        try:
            response = yield from protocol.request(request).response
        except Exception as e:
            print('Failed to fetch resource:')
            print(e)
        else:
            #print('Result: %s\n%r'%(response.code, response.payload))
            #Get the RSSI value from the payload
            #print(response.payload.decode('ascii'))
			with open('data.txt', 'w') as f:
				f.write(response.payload.decode('ascii'))

        x=x+1
        #put into sleep
        time.sleep(reading_interval)

if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(main())
