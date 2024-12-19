<?php
/*
 * Copyright (c) 2015-2016 Nexmotion, Inc.
 * All rights reserved.
 *
 * REVISION HISTORY (reverse chronological order)
 *=============================================================================
 * 2016/09/02 엄준현 수정(원파일업체 파일업로드 부분 중복되던거 수정)
 * 2016/10/12 엄준현 수정(가격 표시 로직 수정)
 *=============================================================================
 *
 */

/**
 * @param 주문서 작성 html 생성
 *
 * @param $conn = connection identifier
 * @param $dao  = 후공정, 사진경로 검색용
 * @param $rs   = 검색결과
 * @param $util = 후공정 정보 생성용 유틸
 * @param $price_info_arr = 가격정보배열
 *
 * return 주문서 작성 html
 */
function makeSheetOrderListHtml($conn, $dao, $rs, $util,
                                $emergency_order, &$price_info_arr) {
    $ret  = "<table class=\"list info _details\" id=\"table_1\">";
    $ret .= "    <colgroup>";
    $ret .= "        <col class=\"no\">";
    $ret .= "        <col class=\"date\">";
    $ret .= "        <col class=\"productDetails\">";
    $ret .= "        <col class=\"post\">";
    $ret .= "        <col class=\"amount\">";
    $ret .= "        <col class=\"price\">";
    $ret .= "    </colgroup>";
    $ret .= "    <thead>";
    $ret .= "        <tr>";
    $ret .= "            <th>번호</th>";
    $ret .= "            <th>담은날</th>";
    $ret .= "            <th>상품정보</th>";
    $ret .= "            <th>후가공</th>";
    $ret .= "            <th>수량&times;건</th>";
    $ret .= "            <th>결제예정금액</th>";
    $ret .= "        </tr>";
    $ret .= "    </thead>";
    $ret .= "    <tbody>";

    $upper_tr  = "<tr id=\"tr_%s\">"; //#0
    $upper_tr .= "    <td>";
    $upper_tr .= "        <span class=\"idx\" idx=\"%s\">%s</span>"; //#1 순번
    $upper_tr .= "        <input type=\"hidden\" name=\"seq[]\" value=\"%s\" />"; //#2 일련번호
    $upper_tr .= "        <input type=\"hidden\" name=\"order_num_%s\" class=\"order_num\" value=\"%s\" />"; //#2-1 주문번호
    $upper_tr .= "    </td>";
    $upper_tr .= "    <td>%s</td>"; //#3 담은날
    $upper_tr .= "    <td  class=\"_name\" id=\"title_td_%s\" style=\"text-align:left;\">"; //#4
    $upper_tr .=          "<div class=\"subject\">";
    $upper_tr .=              "<span class=\"category_text\">[%s]</span>&nbsp;"; //#5 카테고리명
    $upper_tr .=              "<span class=\"order_list_title_text\">%s</span><br>"; //#6 주문제목
    $upper_tr .=              "<ul class=\"information\">";
    $upper_tr .=                  "%s"; //#7 주문상세
    $upper_tr .=              "</ul>";
    $upper_tr .=          "</div>";
    $upper_tr .= "    </td>";
    $upper_tr .= "    <td>";
    $upper_tr .=          "<div class=\"subject\">";
    $upper_tr .=              "<ul class=\"information\">";
    $upper_tr .=                  "%s"; //#8 후가공 내역
    $upper_tr .=              "</ul>";
    $upper_tr .=          "</div>";
    $upper_tr .= "    </td>";
    $upper_tr .= "    <td>%s%s(%s건)</td>"; //#9 amt, amt_unit_dvs, count
    $upper_tr .= "    <td>";
    $upper_tr .= "        %s 원"; //#10 sell_price
    $upper_tr .= "    </td>";
    $upper_tr .= "    <input type=\"hidden\" id=\"sell_price_%s\" name=\"sell_price_%s\" class=\"sell_price\" value=\"%s\" />"; //#12 sell_price
    $upper_tr .= "    <input type=\"hidden\" id=\"grade_sale_price_%s\" name=\"grade_sale_price_%s\" class=\"grade_sale_price\" value=\"%s\" />"; //#13 grade_sale_price
    $upper_tr .= "    <input type=\"hidden\" id=\"member_sale_price_%s\" name=\"member_sale_price_%s\" class=\"member_sale_price\" value=\"%s\" />"; //#14-1 member_sale_price
    $upper_tr .= "    <input type=\"hidden\" id=\"event_price_%s\" name=\"event_price_%s\" class=\"event_price\"  value=\"%s\" />"; //#15 event_price
    $upper_tr .= "    <input type=\"hidden\" id=\"cate_sortcode_%s\" name=\"cate_sortcode_%s\" value=\"%s\" />"; //#16 cate_sortcode
    $upper_tr .= "    <input type=\"hidden\" id=\"emergency_order_%s\" name=\"emergency_order\" value=\"%s\" />"; //#17 order_common_seqno, depth1
    $upper_tr .= "    <input type=\"hidden\" id=\"dlvr_price_%s\" name=\"dlvr_price_%s\" />"; //#17 order_common_seqno, depth1
    // after_use_yn ?
    $upper_tr .= "</tr>";

    $sum_sell_price       = 0;
    $sum_grade_sale_price = 0;
    $sum_event_price      = 0;
    $sum_add_opt_price    = 0;
    $sum_add_after_price  = 0;

    $i = 1;
    while ($rs && !$rs->EOF) {
        $fields = $rs->fields;

        $cate_sortcode = $fields["cate_sortcode"];

        $order_common_seqno = $fields["order_common_seqno"];
        $order_num          = $fields["order_num"];
        $order_regi_date    = $fields["order_regi_date"];
        $title              = $fields["title"];
        $order_detail       = $fields["order_detail"];
        $sell_price         = doubleval($fields["sell_price"]);
        $grade_sale_price   = doubleval($fields["grade_sale_price"]);
        $member_sale_price  = doubleval($fields["member_sale_price"]);
        $add_opt_price      = doubleval($fields["add_opt_price"]);
        $add_after_price    = doubleval($fields["add_after_price"]);
        $event_price        = doubleval($fields["event_price"]);
        $expec_weight       = $fields["expec_weight"];
        $amt                = $fields["amt"];
        $amt_unit_dvs       = $fields["amt_unit_dvs"];
        $count              = $fields["count"];

        $cate_name = trim(explode(',', (explode('/', $order_detail)[0]))[0]);
        $order_detail = explode('/', $order_detail, 2)[1];

        $aft_rs = $dao->selectOrderAfterHistory(
            $conn,
            ["order_common_seqno" => $order_common_seqno]
        );

        $after_info = '';
        while ($aft_rs && !$aft_rs->EOF) {
            $fields = $aft_rs->fields;

            if ($fields["basic_yn"] === 'Y') {
                $aft_rs->MoveNext();
                continue;
            }

            $aft = sprintf("%s(%s)", $fields["name"]
                , $util->getOptAfterFullName($fields));

            $after_info .= $aft . " / ";

            $aft_rs->MoveNext();
        }
        $after_info = substr($after_info, 0, -2);
        $opt_rs = $dao->selectOrderOptHistory(
            $conn,
            ["order_common_seqno" => $order_common_seqno]
        );
        $opt_info = '';
        while ($opt_rs && !$opt_rs->EOF) {
            $fields = $opt_rs->fields;

            if ($fields["basic_yn"] === 'Y') {
                $opt_rs->MoveNext();
                continue;
            }

            $opt = sprintf("%s(%s)", $fields["name"]
                , $util->getOptAfterFullName($fields));

            $opt_info .= $opt . " / ";

            $opt_rs->MoveNext();
        }
        $after_info = $after_info . substr($opt_info, 0, -2);

        $calc_sum_price = $sell_price;

        // 가격합산 정보 생성
        $sum_sell_price        += $sell_price;
        $sum_grade_sale_price  += $grade_sale_price;
        $sum_member_sale_price += $member_sale_price;
        $sum_event_price       += $event_price;
        $sum_add_opt_price     += $add_opt_price;
        $sum_add_after_price   += $add_after_price;

        // 당일판 depth1
        $depth1 = $emergency_order[$order_common_seqno] ?? '';

        // 전체 html 생성
        $ret .= sprintf($upper_tr, $i                                //#0
            , $i                                //#1
            , $i                                //#1
            , $order_common_seqno               //#2
            , $order_common_seqno               //#2-1
            , $order_num                        //#2-1
            , explode(' ', $order_regi_date)[0] //#3
            , $order_common_seqno               //#4
            , $cate_name                        //#5
            , $title                            //#6
            , $order_detail                     //#7
            , $after_info                       //#8
            , $amt                              //#9
            , $amt_unit_dvs                     //#9
            , $count                            //#9
            , number_format($calc_sum_price)    //#10
            , $order_common_seqno               //#12
            , $order_common_seqno               //#12
            , $sell_price                  //#12
            , $order_common_seqno               //#13
            , $order_common_seqno               //#12
            , $grade_sale_price                 //#13
            , $order_common_seqno               //#12
            , $order_common_seqno               //#14
            , $member_sale_price                //#14
            , $order_common_seqno               //#12
            , $order_common_seqno               //#15
            , $event_price                      //#15
            , $order_common_seqno			     //#16
            , $order_common_seqno			     //#16
            , $cate_sortcode                    //#16
            , $order_common_seqno               //#17
            , $depth1                           //#17
            , $order_common_seqno               //#17
            , $order_common_seqno               //#17
        );

        $i++;
        $rs->MoveNext();
    }

    $ret .= "</tbody>";
    $ret .= "</table>";

    $price_info_arr["sell"]  = $sum_sell_price;
    $price_info_arr["grade"] = $sum_grade_sale_price;
    $price_info_arr["member"] = $sum_member_sale_price;
    $price_info_arr["event"] = $sum_event_price;
    $price_info_arr["sum"] = $sum_sell_price;

    return $ret;
}
