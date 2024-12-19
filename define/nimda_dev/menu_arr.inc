<?php
define("MEMBER" , "01");
define("ESTI"   , "02");
define("SALES"  , "03");
define("MARKET" , "04");
define("ORDER"  , "05");
define("RECEIPT", "06");
define("TYPSET" , "07");
define("OUTPUT" , "08");
define("PRT"    , "09");
define("AFT_B"  , "10");
define("AFT_O"  , "11");
define("STOR"   , "12");
define("RELEASE", "13");
define("DLVR"   , "14");
define("CALCUL" , "15");

define("SETTING", "90");
define("STATS"  , "91");

define("TOP_MENU", [
    MEMBER => [
         "url" => "/member"
         ,"name" => "회원관리"
    ]
    ,ESTI => [
        "url" => "/esti"
        ,"name" => "견적관리"
    ]
    ,SALES => [
        "url" => "/sales"
        ,"name" => "매출관리"
    ]
    ,MARKET => [
        "url" => ""
        ,"name" => "마케팅관리"
    ]
    ,ORDER => [
        "url" => "/order"
       ,"name" => "주문"
    ]
    ,RECEIPT => [
        "url" => ""
        ,"name" => "접수"
    ]
    ,TYPSET => [
        "url" => ""
        ,"name" => "조판"
    ]
    ,OUTPUT => [
        "url" => ""
        ,"name" => "출력"
    ]
    ,PRT => [
        "url" => ""
        ,"name" => "인쇄"
    ]
    ,AFT_B => [
        "url" => ""
        ,"name" => "기본후가공"
    ]
    ,AFT_O => [
        "url" => ""
        ,"name" => "주문후가공"
    ]
    ,STOR => [
        "url" => ""
        ,"name" => "입고"
    ]
    ,RELEASE => [
        "url" => "/release"
        ,"name" => "출고"
    ]
    ,DLVR => [
        "url" => "/dlvr"
        ,"name" => "배송"
    ]
    ,CALCUL => [
        "url" => ""
        ,"name" => "정산관리"
    ]
    ,SETTING => [
        "url" => "/setting"
        ,"name" => "환경설정"
    ]
    ,STATS => [
        "url" => "/stats"
        ,"name" => "통계"
    ]
]);

define("SIDE_MENU", [
    // 회원관리
    MEMBER => [
        '0' => [
            "name" => "회원관리"
            ,"01" => [
                 "url" => "/member_common_list.html"
                 ,"name" => "회원통합리스트"
            ]
            ,"02" => [
                "url" => "/quiescence_list.html"
                ,"name" => "휴면대상회원리스트"
                ,"new" => true
            ]
            ,"03" => [
                "url" => "/reduce_list.html"
                ,"name" => "정리회원리스트"
            ]
        ]
    ]
    // 견적관리
    ,ESTI => [
        '0' => [
            "name" => "견적관리"
            ,"01" => [
                "url" => "/esti_list.html"
                ,"name" => "견적리스트"
            ]
            ,"02" => [
                "url" => "/esti_stc.html"
                ,"name" => "견적통계"
            ]
        ]
    ]
    // 매출관리
    ,SALES => [
         '0' => [
            "name" => "고객관리"
            ,"01" => [
                "url" => "/sales_mng.html"
                ,"name" => "고객정보검색"
            ]
            ,"02" => [
                "url" => "/.html"
                ,"name" => "고객정보리스트"
            ]
            ,"03" => [
                "url" => "/sales_sales_info.html"
                ,"name" => "매출거래현황정보"
            ]
            ,"04" => [
                "url" => "/sales_order_info.html"
                ,"name" => "주문정보"
            ]
            ,"05" => [
                "url" => "/sales_order_status.html"
                ,"name" => "주문진행정보"
            ]
            ,"06" => [
                "url" => "/sales_manu_limit.html"
                ,"name" => "생산 투입한도조회"
            ]
            ,"07" => [
                "url" => "/sales_crm_info.html"
                ,"name" => "CRM정보"
            ]
            ,"08" => [
                "url" => "/sales_new_member.html"
                ,"name" => "신규회원정보"
            ]
            ,"09" => [
                "url" => "/sales_prdt_info.html"
                ,"name" => "품목별현황정보"
            ]
        ]
        ,'1' => [
            "name" => "결산관리"
            ,"10" => [
                "url" => "/group_mailing.html"
                ,"name" => "고객그룹메일링"
            ]
            ,"11" => [
                "url" => "/.html"
                ,"name" => "그룹메일링"
            ]
            ,"12" => [
                "url" => "/mailing_result.html"
                ,"name" => "메일링이력"
            ]
            ,"13" => [
                "url" => "/group_sms.html"
                ,"name" => "SMS전송"
            ]
        ]
    ]
    ,MARKET => [
    ]
    ,ORDER => [
        '0' => [
            "name" => "주문관리"
            ,"01" => [
                "url" => "/order_mng.html"
                ,"name" => "고객별주문현황"
            ]
        ]
        ,'1' => [
            "name" => "사고처리"
            ,"02" => [
                "url" => "/order_process_list_01.html"
                ,"name" => "처리요청목록"
            ]
            ,"03" => [
                "url" => "/order_process_list_02.html"
                ,"name" => "처리불가목록"
            ]
            ,"04" => [
                "url" => "/order_process_list_03.html"
                ,"name" => "처리완료목록"
            ]
            ,"05" => [
                "url" => "/order_act_st.html"
                ,"name" => "사고처리통계"
            ]
        ]
        ,'2' => [
            "name" => "진행관리"
            ,"06" => [
                "url" => "/order_cart_info.html"
                ,"name" => "장바구니현황"
            ]
            ,"07" => [
                "url" => "/order_deposit_wait.html"
                ,"name" => "입금대기현황"
            ]
            ,"08" => [
                "url" => "/order_cancel.html"
                ,"name" => "주문취소"
            ]
            ,"09" => [
                "url" => "/order_receipt_status.html"
                ,"name" => "접수중 현황"
            ]
            ,"10" => [
                "url" => "/order_typeset_status.html"
                ,"name" => "조판중 현황"
            ]
            ,"11" => [
                "url" => "/order_prdt_delivery.html"
                ,"name" => "생산중 배송관리"
            ]
        ]
    ]
    ,RECEPIT => [
        '0' => [
            "name" => ""
            ,"01" => [
                "url" => "/.html"
                ,"name" => ""
            ]
        ]
    ]
    ,TYPSET => [
    ]
    ,OUTPUT => [
    ]
    ,PRT => [
    ]
    ,AFT_B => [
    ]
    ,AFT_O => [
    ]
    ,STOR => [
    ]
    ,RELEASE => [
        '0' => [
            "name" => "출고"
            ,"01" => [
                "url" => "/release_dlvr_status.html"
                ,"name" => "생산중배송현황"
            ]
            ,"02" => [
                "url" => "/release_direct.html"
                ,"name" => "직배송"
            ]
            ,"03" => [
                "url" => "/release_delivery.html"
                ,"name" => "택배(화물)"
            ]
            ,"04" => [
                "url" => "/release_quick.html"
                ,"name" => "퀵서비스(기타)"
            ]
            ,"05" => [
                "url" => "/release_visit01.html"
                ,"name" => "필동출고"
            ]
            ,"06" => [
                "url" => "/release_visit02.html"
                ,"name" => "인현출고"
            ]
            ,"07" => [
                "url" => "/release_group_dlvr.html"
                ,"name" => "묶음배송확인"
            ]
        ]
    ]
    ,DLVR => [
        '0' => [
            "name" => "배송업체정보"
            ,"01" => [
                "url" => "/dlvr_company_list.html"
                ,"name" => "배송업체리스트"
            ]
        ]
        ,'1' => [
            "name" => "배송완료"
            ,"02" => [
                "url" => "/dlvr_complete_list.html"
                ,"name" => "배송별조회리스트"
            ]
        ]
    ]
    ,CALCUL => [
    ]
    ,SETTING => [
        '0' => [
            "name" => "기초정보관리"
            ,'0' => [
                "name" => "상품관리"
                ,"01" => [
                    "url" => "/set_info_basic_mng.html"
                    ,"name" => "기본정보관리"
                ]
                ,"02" => [
                    "url" => "/set_info_category.html"
                    ,"name" => "분류체계(카테고리) 관리"
                ]
                ,"03" => [
                    "url" => "/set_info_prdt_mng.html"
                    ,"name" => "상품구성관리"
                ]
            ]
            ,'1' => [
                "name" => "상품가격관리"
                ,"04" => [
                    "url" => "/set_info_prdt_price.html"
                    ,"name" => "상품가격리스트"
                ]
                ,"05" => [
                    "url" => "/set_info_after_price.html"
                    ,"name" => "후가공가격리스트"
                ]
                ,"06" => [
                    "url" => "/set_info_option_price.html"
                    ,"name" => "옵션가격리스트"
                ]
                ,"07" => [
                    "url" => "/set_info_calculation_price.html"
                    ,"name" => "계산형가격리스트"
                ]
            ]
            ,'2' => [
                "name" => "생산품목관리"
                ,"08" => [
                    "url" => "/set_info_typset_mng.html"
                    ,"name" => "조판관리"
                ]
                ,"09" => [
                    "url" => "/set_info_paper_mng.html"
                    ,"name" => "종이관리"
                ]
                ,"10" => [
                    "url" => "/set_info_output_mng.html"
                    ,"name" => "출력관리"
                ]
                ,"11" => [
                    "url" => "/set_info_print_mng.html"
                    ,"name" => "인쇄관리"
                ]
                ,"12" => [
                    "url" => "/set_info_after_mng.html"
                    ,"name" => "후가공관리"
                ]
                ,"13" => [
                    "url" => "/set_info_option_mng.html"
                    ,"name" => "옵션관리"
                ]
            ]
            ,'3' => [
                "name" => "협력업체관리"
                ,"14" => [
                    "url" => "/set_info_partner_list.html"
                    ,"name" => "협력업체리스트"
                ]
                ,"15" => [
                    "url" => "/set_info_partner_regi.html"
                    ,"name" => "협력업체등록"
                ]
                ,"16" => [
                    "url" => "/set_info_partner_mng.html"
                    ,"name" => "협력업체 품목관리"
                ]
            ]
        ]
        ,'1' => [
            "name" => "운영관리"
            ,'0' => [
            ]
        ]
        ,'2' => [
            "name" => "고객관리"
            ,'0' => [
            ]
        ]
        ,'3' => [
            "name" => "사용자관리"
            ,'0' => [
            ]
        ]
    ]
    ,STATS => [
        '0' => [
            "name" => "통계"
            ,'0' => [
                "name" => "통계관리"
                ,"01" => [
                    "url" => "/.html"
                    ,"name" => "통계"
                ]
            ]
        ]
    ]
]);
