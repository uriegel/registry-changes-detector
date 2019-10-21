{
    "targets": [{
        "target_name": "registry-changes-detector",
        "sources": [ 
            'addon.cpp'
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "<!@(node -p \"var a = require('node-addon-api').include; var b = a.substr(0, a.length - 15); b + 'event-source-base' + a[a.length-1]\")"
        ],
        'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
        "cflags": ["-Wall", "-std=c++17"],
        'cflags_cc': ["-Wall", "-std=c++17"],
        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        'link_settings': {
            "libraries": [ 
            ]
        },            
        'conditions': [
            ['OS=="win"', {
                'defines': ['WINDOWS'],
                "msvs_settings": {
                    "VCCLCompilerTool": {
                        "ExceptionHandling": 1
                    }
                },
                "sources": [ 
                    'registry.cpp'
                ]                
            }]
        ]          
    }]
}

