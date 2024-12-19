<?
//작업가이드 메뉴목록(해쉬맵)
const WORK_GUIDE_MENU_ARR = [
    "ad"  => "상업인쇄"
    ,"dt"  => "디지털인쇄"
    ,"ao"  => "실사출력"
    ,"nc"  => "명함"
    ,"st"  => "스티커"
    ,"bl"  => "전단"
    ,"ev"  => "봉투"
    ,"mt"  => "경인쇄"
    ,"etc" => "기타인쇄"
    ,"gb"  => "그린백"
];
const WORK_GUIDE_CATE_MENU_ARR = [
    "ad"  => [
        "ad_cat" => "카탈로그/브로셔"
        ,"ad_leaf" => "리플렛/팸플릿"
        ,"ad_pos" => "포스터"
        ,"ad_hol" => "홀더"
    ]
    ,"dt"  => [
        "dt_nc" => "디지털명함"
        ,"dt_ev" => "디지털엽서"
        ,"dt_bl" => "디지털전단지"
        ,"dt_po" => "디지털포스터"
    ]
    ,"ao"  => [
        "ao_pet" => "PET배너"
        ,"ao_mes" => "메쉬배너"
        ,"ao_tra" => "투명배너"
        ,"ao_min" => "미니배너"
        ,"ao_pla" => "현수막"
        ,"ao_gue" => "게릴라 현수막"
        ,"ao_scr" => "족자 현수막"
        ,"ao_ful" => "등신대"
        ,"ao_rac" => "배너거치대"
    ]
    ,"nc"  => [
        "nc_nor" => "일반명함"
        ,"nc_spe" => "수입명함"
        ,"nc_car" => "카드명함"
    ]
    ,"st"  => [
        "st_nor" => "컬러스티커"
        ,"st_spe" => "특수지스티커"
        ,"st_tom" => "도무송스티커"
        ,"st_fre" => "자유형도무송"
    ]
    ,"bl"  => [
        "bl_ply" => "합판전단지"
        ,"bl_mon" => "독판전단지"
        ,"bl_min" => "초소량인쇄"
    ]
    ,"ev"  => [
        "ev_col" => "컬러봉투"
        ,"ev_mas" => "마스터봉투"
    ]
    ,"mt"  => [
        "mt_ncr" => "NCR지"
        ,"mt_for" => "양식지"
    ]
    ,"etc" => [
        "etc_lot" => "복권인쇄"
        ,"etc_men" => "메뉴판"
        ,"etc_mul" => "문어발"
        ,"etc_doo" => "문고리"
        ,"etc_mag" => "자석판촉물"
        ,"etc_mem" => "메모지"
    ]
    ,"gb"  => [
        "gb_mty" => "M타입 봉투"
        ,"gb_squ" => "사각봉투"
    ]
];

//이용안내 메뉴목록(해쉬맵)
//171109 이청산 수정(디자이너 요청)
const GUIDE_MENU_HASH = array(
    "membership"             => "회원",
    "order"                  => "주문",
    "print_warning"          => "인쇄주의사항",
    "manual_esti_guide"      => "수동견적가이드",
    "release_delivery_guide" => "출고,배송안내",
    "claim"                  => "클레임",
    "paper_dscr"             => "재질설명"
  //  "after_dscr"             => "후공정"
);

//이용안내 메뉴목록(배열)
//171109 이청산 수정(디자이너 요청)
const GUIDE_MENU_ARR = array(
    array("membership"            ,"회원"),
    array("order"                 ,"주문"),
    array("print_warning"         ,"인쇄주의사항"),
    array("manual_esti_guide"     ,"수동견적가이드"),
    array("release_delivery_guide","출고,배송안내"),
    array("claim"                 ,"클레임"),
    array("paper_dscr"            ,"재질설명")
   // array("after_dscr"            ,"후공정")
);

//FAQ 메뉴목록(해쉬맵)
const FAQ_MENU_HASH = array(
    "membership" => "회원",
    "order" => "주문",
    "event" => "이벤트",
    "pointcoupon" => "포인트/쿠폰",
    "estimate" => "견적",
    "production" => "생산",
    "delivery" => "배송",
    "claim" => "클레임",
    "accounting" => "회계",
    "etc" => "기타"
);

//FAQ 메뉴목록(배열)
const FAQ_MENU_ARR = array(
    array("membership","회원"),
    array("order","주문"),
    array("event","이벤트"),
    array("pointcoupon","포인트/쿠폰"),
    array("estimate","견적"),
    array("production","생산"),
    array("delivery","배송"),
    array("claim","클레임"),
    array("accounting","회계"),
    array("etc","기타")
);

define("WORK_GUIDE_ARR", [
    // 상업인쇄
    "001001001" => "ad_cat"
    ,"001002001" => "ad_leaf"
    ,"001003001" => "ad_pos"
    ,"001004001" => "ad_hol"
    // 디지털인쇄
    ,"011001001" => "dt_nc"
    ,"011002001" => "dt_ev"
    ,"011002002" => "dt_bl"
    ,"011003001" => "dt_po"
    // 실사출력(추가, 수정필요)
    ,"002002001" => "ao_pet"
    ,"002002002" => "ao_tra"
    ,"002002003" => "ao_mes"
    ,"002002004" => "ao_pet" // 가로등배너 필요시 추가필요
    ,"002001001" => "ao_min"
    ,"002005001" => "ao_gue"
    ,"002005002" => "ao_scr"
    ,"002005003" => "ao_pla"
    ,"002004001" => "ao_ful"
    ,"002003001" => "ao_rac"
    // 명함
    ,"003001001" => "nc_nor"
    ,"003001002" => "nc_nor"
    ,"003001003" => "nc_nor"
    ,"003001004" => "nc_nor"
    ,"003002001" => "nc_spe"
    ,"003003001" => "nc_car"
    // 스티커
    ,"004001001" => "st_nor"
    ,"004002001" => "st_spe"
    ,"004003001" => "st_tom"
    ,"004003002" => "st_tom"
    ,"004003003" => "st_tom"
    ,"004003004" => "st_tom"
    ,"004003005" => "st_tom"
    ,"004003006" => "st_tom"
    ,"004003007" => "st_tom"
    ,"004003008" => "st_tom"
    ,"004003009" => "st_fre"
    // 전단 
    ,"005001001" => "bl_ply"
    ,"005002001" => "bl_mon"
    ,"005003001" => "bl_min"
    // 봉투
    ,"006001001" => "ev_col"
    ,"006001002" => "ev_col"
    ,"006001003" => "ev_col"
    ,"006001004" => "ev_col"
    ,"006001005" => "ev_col"
    ,"006001006" => "ev_col"
    ,"006002001" => "ev_mas"
    ,"006002002" => "ev_mas"
    ,"006002003" => "ev_mas"
    ,"006002004" => "ev_mas"
    ,"006002005" => "ev_mas"
    ,"006002006" => "ev_mas"
    ,"006002007" => "ev_mas"
    ,"006002008" => "ev_mas"
    ,"006002009" => "ev_mas"
    ,"006002010" => "ev_mas"
    // 경인쇄
    ,"007001001" => "mt_ncr"
    ,"007001002" => "mt_ncr"
    ,"007001003" => "mt_ncr"
    ,"007002001" => "mt_for"
    // 기타인쇄
    ,"008001001" => "etc_lot"
    ,"008001002" => "etc_men"
    ,"008001003" => "etc_mul"
    ,"008001004" => "etc_mul"
    ,"008001005" => "etc_doo"
    ,"008001006" => "etc_mem"
    ,"008002001" => "etc_mag"
    ,"008002002" => "etc_mag"
    ,"008002003" => "etc_mag"
    // 그린백 
    ,"009001001" => "gb_mty"
    ,"009001002" => "gb_squ"
     
]);
?>
