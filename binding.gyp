{
	"targets": [
		{
			"target_name": "2wire",
			"sources": [ "2wire.cc", "TwiDevice.cc", "TwiDevice_Wrap.cc" ],
			"cflags_cc!": [ "-fno-exceptions" ],
			"cflags": [ "-std=c++11", "-fexceptions" ],
			"include_dirs" : [
 	 			"<!(node -e \"require('nan')\")",
				"<!(node -e \"require('nan-check')\")",
				"<!(node -e \"require('nan-marshal')\")"
			]
		}
	],
}
