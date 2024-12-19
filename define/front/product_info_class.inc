<?
/*
 * Copyright (c) 2015-2017 Nexmotion, Inc.
 * All rights reserved.
 * 
 * REVISION HISTORY (reverse chronological order)
 *=============================================================================
 * 
 *=============================================================================
 *
 */
class ProductInfoClass {

    // 2023년 휴일
    const HOLODAYS = [
        '2023-07-31',
        '2023-08-01',
        '2023-08-02',
        '2023-08-15',
        '2023-09-28',
        '2023-09-29',
        '2023-10-02',
        '2023-10-03',
        '2023-10-09',
        '2023-12-25',
    ];
    // 후공정 종류 배열
    const AFTER_ARR = [
         "코팅"       => "coating"
        ,"코팅(판)"       => "coating"
        ,"봉투가공"       => "enveloping"
        ,"귀도리"     => "rounding"
        ,"오시"       => "impression"
        ,"도무송오시" => "thomson_impression"
        ,"미싱"       => "dotline"
        ,"타공"       => "punching"
        ,"접지"       => "foldline"
        ,"엠보싱"     => "embossing"
        ,"박"         => "foil"
        ,"형압"       => "press"
        ,"도무송"     => "thomson"
        ,"도무송(판)"     => "thomson"
        ,"넘버링"     => "numbering"
        ,"재단"       => "cutting"
        ,"제본"       => "binding"
        ,"접착"       => "bonding"
        ,"가공"       => "manufacture"
        ,"복권실크"   => "lotterysilk"
        ,"라미넥스"   => "laminex"
        ,"후지반칼"   => "halfknife"
        ,"전체빼다"   => "background"
        ,"추가미싱"   => "add_dotline"
        ,"열재단"     => "heat_cutting"
        ,"고리"       => "ring"
        ,"족자"       => "scroll"
        ,"쿨코팅"     => "cool_coating"
        ,"엣지박"     => "edgefoil"
    ];

    // 작업, 재단사이즈간 간격, '간격 = 작업 - 재단'이 아닌경우 작성
    const SIZE_GAP = [];

    // 상품별 리다이렉트 페이지
    const PAGE_ARR = [
        // 상업인쇄
         "001" => [
             "001001001" => "ad_catabro.html"
            ,"001002001" => "ad_stan.html"
            ,"001003001" => "ad_poster.html"
            ,"001004001" => "ad_holder.html"
        ] 
        // 실사출력
        ,"002" => [
             "002003001" => "ao_rack_info.html"
            ,"002005003" => "ao_placard.html"
            ,"ELSE" => "ao.html"
        ] 
        // 명함
        ,"003" => [
             "003003001" => "nc_card.html"
            ,"003003002" => "nc_card.html"
            ,"ELSE" => "nc.html"
        ] 
        // 스티커
        ,"004" => [
             "004003" => "st_thomson.html"
            ,"004003009" => "st_free_thomson.html"
            ,"ELSE" => "st_cut.html"
        ] 
        // 전단
        ,"005" => [
             "005001001" => "bl.html"
            ,"005001002" => "bl.html"
            ,"005001003" => "bl.html"
            ,"005001004" => "bl.html"
            ,"005002001" => "bl_mono.html"
            ,"005003001" => "bl_small.html"
        ] 
        // 봉투
        ,"006" => [
             "006001" => "ev.html"
            ,"006002" => "ev_master.html"
            ,"006003" => "ev.html"
        ] 
        // 경인쇄
        ,"007" => [
             "007001" => "mt_ncr.html"
            ,"007002001" => "mt_form.html"
        ] 
        // 기타인쇄
        ,"008" => [
             "008001001" => "etc_lottery.html"
            ,"008001002" => "etc_menu.html"
            ,"008001003" => "etc_multiple.html"
            ,"008001004" => "etc_multiple.html"
            ,"008001005" => "etc_door.html"
            ,"008001006" => "etc_memo.html"
            ,"008002001" => "mg_paper.html"
            ,"008002002" => "mg_sticker.html"
            ,"008002003" => "mg_opener.html"
        ] 
        // 그린백
        ,"009" => "gb.html"
        // 별도견적
        ,"010" => [
             "010001001" => "esti_sheet.html"
            ,"010001002" => "esti_booklet.html"
        ] 
        // 디지털인쇄
        ,"011" => [
             "011001001" => "dt_nc.html"
            ,"011002001" => "dt_bl.html"
            ,"011002002" => "dt_bl.html"
            ,"011003001" => "dt_nc.html"
        ] 
    ];
}
?>
