<?php
// 거치대 depth1 한글=>영문 배열
define("RACK_NAME_EN", [
     "실내용거치대"   => "inner_rack"
    ,"실외용거치대"   => "outer_rack"
    ,"미니배너거치대" => "mini_rack"
]);

// 거치대 최소/최대 사이즈
define("RACK_MIN_MAX_SIZE", [
    "실외용거치대" => [
        "제로 배너" => [
            '단면' => [
                 "MIN_WID" => 450, "MIN_VERT" => 1300
                ,"MAX_WID" => 700, "MAX_VERT" => 1900
            ]
            ,'양면' => [
                 "MIN_WID" => 450, "MIN_VERT" => 1300
                ,"MAX_WID" => 700, "MAX_VERT" => 1900
            ]
        ]
        ,"샵플러스 배너" => [
            '단면' => [
                 "MIN_WID" => 500, "MIN_VERT" => 1500
                ,"MAX_WID" => 700, "MAX_VERT" => 1900
            ]
            ,'양면' => [
                 "MIN_WID" => 500, "MIN_VERT" => 1500
                ,"MAX_WID" => 700, "MAX_VERT" => 1900
            ]
        ]
        ,"세븐 배너" => [
            '단면' => [
                 "MIN_WID" => 450, "MIN_VERT" => 1300
                ,"MAX_WID" => 700, "MAX_VERT" => 1900
            ]
            ,'양면' => [
                 "MIN_WID" => 450, "MIN_VERT" => 1300
                ,"MAX_WID" => 700, "MAX_VERT" => 1900
            ]
        ]
        ,"포인트 배너" => [
            '단면' => [
                 "MIN_WID" => 450, "MIN_VERT" => 1300
                ,"MAX_WID" => 700, "MAX_VERT" => 1900
            ]
            ,'양면' => [
                 "MIN_WID" => 450, "MIN_VERT" => 1300
                ,"MAX_WID" => 700, "MAX_VERT" => 1900
            ]
        ]
        ,"스마트 배너" => [
            '단면' => [
                 "MIN_WID" => 450, "MIN_VERT" => 1300
                ,"MAX_WID" => 700, "MAX_VERT" => 1900
            ]
            ,'양면' => [
                 "MIN_WID" => 450, "MIN_VERT" => 1300
                ,"MAX_WID" => 700, "MAX_VERT" => 1900
            ]
        ]
    ]

    ,"실내용거치대" => [
        "이벤트 배너" => [
            '-' => [
                 "MIN_WID" => 600, "MIN_VERT" => 1800
                ,"MAX_WID" => 600, "MAX_VERT" => 1800
            ]
        ]
        ,"세일 배너" => [
            '-' => [
                 "MIN_WID" => 600, "MIN_VERT" => 1800
                ,"MAX_WID" => 600, "MAX_VERT" => 1800
            ]
        ]
        ,"원스톱 배너" => [
            '-' => [
                 "MIN_WID" => 600, "MIN_VERT" => 1800
                ,"MAX_WID" => 600, "MAX_VERT" => 1800
            ]
        ]
        ,"포인트 배너" => [
            '-' => [
                 "MIN_WID" => 600, "MIN_VERT" => 1300
                ,"MAX_WID" => 600, "MAX_VERT" => 1800
            ]
        ]
        ,"쇼배너" => [
            "소형" => [
                 "MIN_WID" => 500, "MIN_VERT" => 1600
                ,"MAX_WID" => 500, "MAX_VERT" => 1600
            ]
            ,"중형" => [
                 "MIN_WID" => 600, "MIN_VERT" => 1800
                ,"MAX_WID" => 600, "MAX_VERT" => 1800
            ]
            ,"대형" => [
                 "MIN_WID" => 1500, "MIN_VERT" => 2500
                ,"MAX_WID" => 1500, "MAX_VERT" => 2500
            ]
        ]
    ]

    ,"미니배너거치대" => [
        "미니배너거치대" => [
             "MIN_WID" => 0, "MIN_VERT" => 0
            ,"MAX_WID" => 0, "MAX_VERT" => 0
        ]
    ]
]);

// 거치대 포장단위 배열
define("RACK_PACKAGE_UNIT", [
     "제로 배너"      => 2
    ,"샵플러스 배너"  => 2
    ,"세븐 배너"      => 2
    ,"포인트 배너"    => 2
    ,"스마트 배너"    => 2
    ,"이벤트 배너"    => 30
    ,"세일 배너"      => 30
    ,"원스톱 배너"    => 15
    ,"포인트 배너"    => 15
    ,"쇼배너"         => 10
    ,"미니배너거치대" => 1
]);
// 거치대 낱개구매시 추가금액 배열
define("RACK_ADD_PRICE", [
     "이벤트 배너" => 1000
    ,"세일 배너"   => 1000
    ,"원스톱 배너" => 500
    ,"포인트 배너" => 500
    ,"쇼배너"      => 500
]);
