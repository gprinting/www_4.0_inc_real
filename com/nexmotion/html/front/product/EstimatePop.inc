<?
/**
 * @brief 팝업 상단 html 생성
 *
 * @param $param = 정보
 *
 * @return 상단 html
 */
function getHtmlTop($param) {
    $html_top  = '';
    $html_top .= "\n<table id=\"esti_cont\" class=\"table_print_esti\" width=\"100%%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">
";
    $html_top .= "\n    <colgroup>";
    $html_top .= "\n        <col style=\"width:50%%;\">";
    $html_top .= "\n        <col style=\"width:50%%;\">";
    $html_top .= "\n    </colgroup>";
    $html_top .= "\n    <tbody>";
    $html_top .= "\n        <tr>";
    $html_top .= "\n            <th colspan=\"2\"><h2>견 적 서</h2></th>";
    $html_top .= "\n        </tr>";
    $html_top .= "\n        <tr>";
    $html_top .= "\n            <tr style=\"vertical-align: top;\">";
    $html_top .= "\n                <td style=\"vertical-align: top;\">";
    $html_top .= "\n                    <table class=\"inner_table_print_esti\">";
    $html_top .= "\n                        <caption>견적일 및 수신</caption>";
    $html_top .= "\n                        <colgroup>";
    $html_top .= "\n                            <col style=\"width:25%%;\">";
    $html_top .= "\n                            <col style=\"\">";
    $html_top .= "\n                        </colgroup>";
    $html_top .= "\n                        <tbody>";
    $html_top .= "\n                            <tr>";
    $html_top .= "\n                                <th>견적일</th>";
    $html_top .= "\n                                <td>%s년 %s월 %s일</td>"; //#0 year, month, day
    $html_top .= "\n                            </tr>";
    $html_top .= "\n                            <tr>";
    $html_top .= "\n                                <th>수&nbsp;&nbsp;신</th>";
    $html_top .= "\n                                <td>%s&nbsp;귀하</td>"; //#1 member_name
    $html_top .= "\n                            </tr>";
    $html_top .= "\n                        </tbody>";
    $html_top .= "\n                    </table>";
    $html_top .= "\n                </td>";
    $html_top .= "\n                <td style=\"vertical-align: top;\">";
    $html_top .= "\n                    <table class=\"inner_table_print_esti print_esti_supplier\">";
    $html_top .= "\n                        <caption>공급자</caption>";
    $html_top .= "\n                        <colgroup>";
    $html_top .= "\n                            <col style=\"width:20%%;\">";
    $html_top .= "\n                            <col style=\"width:30%%;\">";
    $html_top .= "\n                            <col style=\"width:20%%;\">";
    $html_top .= "\n                            <col style=\"width:30%%;\">";
    $html_top .= "\n                        </colgroup>";
    $html_top .= "\n                        <tbody>";
    $html_top .= "\n                            <tr>";
    $html_top .= "\n                                <th>상&nbsp;&nbsp;호</th>";
    $html_top .= "\n                                <td>%s</td>"; //#2 sell_site
    $html_top .= "\n                                <th>대표자</th>";
    $html_top .= "\n                                <td>%s</td>"; //#3 repre_name
    $html_top .= "\n                            </tr>";
    $html_top .= "\n                            <tr>";
    $html_top .= "\n                                <th>주&nbsp;&nbsp;소</th>";
    $html_top .= "\n                                <td colspan=\"3\">%s</td>"; //#4 addr
    $html_top .= "\n                            </tr>";
    $html_top .= "\n                            <tr>";
    $html_top .= "\n                                <th>전화번호</th>";
    $html_top .= "\n                                <td>%s</td>"; //#5 repre_num
    $html_top .= "\n                                <th>팩스</th>";
    $html_top .= "\n                                <td>02-2260-9090</td>";
    $html_top .= "\n                            </tr>";
    $html_top .= "\n                        </tbody>";
    $html_top .= "\n                    </table>";
    $html_top .= "\n                </td>";
    $html_top .= "\n            </tr>";
    /*
    $html_top .= "\n            <tr>";
    $html_top .= "\n                <td colspan=\"2\">";
    $html_top .= "\n                    <table class=\"inner_table_print_esti print_esti_price\">";
    $html_top .= "\n                        <caption style=\"font-weight:100; margin-bottom:6px;\">아래와 같이 견적합니다.</caption>";
    $html_top .= "\n                    </table>";
    $html_top .= "\n                </td>";
    $html_top .= "\n            </tr>";
    $html_top .= "\n        </tr>";
    $html_top .= "\n    </tbody>";
    $html_top .= "\n</table>";
    */

    return sprintf($html_top, $param["year"]             //#0
                            , $param["month"]            //#0
                            , $param["day"]              //#0
                            , $param["member_name"]      //#1
                            , $param["sell_site"]        //#2
                            , $param["repre_name"]       //#3
                            , $param["addr"]             //#4
                            , $param["repre_num"]);      //#5
}

/**
 * @brief 책자형 팝업 중단 html 생성
 *
 * @param $param = 정보
 *
 * @return 중단 html
 */
function getBookletHtmlMid($param) {
    $prdt_dvs_arr = ["표지", "내지1", "내지2", "내지3"];
    $prdt_dvs_arr_count = count($prdt_dvs_arr);

    $cate_name_arr = $param["cate_name_arr"];
    $size_arr      = $param["size_arr"];
    $amt_arr       = $param["amt_arr"];
    $amt_unit_arr  = $param["amt_unit_arr"];
    $count_arr     = $param["count_arr"];
    // 실제 배열로 넘어오는 부분
    $paper_arr     = $param["paper_arr"];
    $page_arr      = $param["page_arr"];
    $tmpt_arr      = $param["tmpt_arr"];
    $after_arr     = $param["after_arr"];
    $pay_price     = $param["pay_price"];
    $supply_price  = $param["supply_price"];
    $tax           = $param["tax"];

    $mid_top  = "\n    <tr>";
    $mid_top .= "\n        <td colspan=\"2\">";
    $mid_top .= "\n            <table class=\"inner_table_print_esti print_esti_price\">";
    $mid_top .= "\n                <caption style=\"font-weight:100; margin-bottom:6px;\">아래와 같이 견적합니다.</caption>";
    $mid_top .= "\n                <colgroup>";
    $mid_top .= "\n                    <col style=\"width:70%%;\">";
    $mid_top .= "\n                    <col style=\"\">";
    $mid_top .= "\n                </colgroup>";
    $mid_top .= "\n                <tbody>";
    $mid_top .= "\n                    <tr>";
    $mid_top .= "\n                        <th scope=\"border-right:none !important; text-align:left; font-weight:100;\">합계금액 \%s 원 + 부가세 \%s 원 + 배송비별도</th>"; //#0 합계금액, 부가세
    $mid_top .= "\n                        <th style=\"border-left:none !important; text-align:right; color:#000\">총 합계금액 : \%s</th>"; //#0 합계금액
    $mid_top .= "\n                    </tr>";
    $mid_top .= "\n                </tbody>";
    $mid_top .= "\n            </table>";
    $mid_top .= "\n        </td>";
    $mid_top .= "\n    </tr>";

    $mid_top = sprintf($mid_top, $supply_price
                               , $tax
                               , $pay_price);

    $mid_mid_base  = "\n<tr>";
    $mid_mid_base .= "\n    <td style=\"vertical-align: top;\">";
    $mid_mid_base .= "\n        <table class=\"inner_table_print_esti\">";
    $mid_mid_base .= "\n            <colgroup>";
    $mid_mid_base .= "\n                <col style=\"width:35%%;\">";
    $mid_mid_base .= "\n                <col style=\"\">";
    $mid_mid_base .= "\n            </colgroup>";
    $mid_mid_base .= "\n            <tbody>";
    $mid_mid_base .= "\n                <tr>";
    $mid_mid_base .= "\n                    <th colspan=\"2\" class=\"table_title\">재질 및 규격</th>";
    $mid_mid_base .= "\n                </tr>";
    $mid_mid_base .= "\n                <tr>";
    $mid_mid_base .= "\n                    <th>품&nbsp;&nbsp;명</th>";
    $mid_mid_base .= "\n                    <td>%s</td>"; //#1 cate_name
    $mid_mid_base .= "\n                </tr>";
    $mid_mid_base .= "\n                <tr>";
    $mid_mid_base .= "\n                    <th>규격</th>";
    $mid_mid_base .= "\n                    <td>%s</td>"; //#2 cate_size
    $mid_mid_base .= "\n                </tr>";
    $mid_mid_base .= "\n                <tr>";
    $mid_mid_base .= "\n                    <th>수량</th>";
    $mid_mid_base .= "\n                    <td>%s%s &times; %s건</td>"; //#3 amt, amt_unit, count
    $mid_mid_base .= "\n                </tr>";

    $mid_mid = sprintf($mid_mid_base, $cate_name_arr[0]
                                    , $size_arr[0]
                                    , $amt_arr[0]
                                    , $amt_unit_arr[0]
                                    , $count_arr[0]);

    // NCR의 경우
    $cate_name = $param["cate_name_arr"][0];
    if ((strpos($cate_name, "NCR") !== false)) {

        $mid_info_base .= "\n            <tr>";
        $mid_info_base .= "\n                <th>재질</th>"; //#0 prdt_dvs
        $mid_info_base .= "\n                <td>%s</td>"; //#1 cate_paper
        $mid_info_base .= "\n            </tr>";

        for ($i = 0; $i < $prdt_dvs_arr_count; $i++) {

            $paper = $paper_arr[$i];
            $page  = $page_arr[$i];

            if (empty($paper)) {
                continue;
            }

            $mid_mid .= sprintf($mid_info_base, $paper);
        }

        $tmpt  = $tmpt_arr[0];

        $mid_info_tmpt .= "\n            <tr>";
        $mid_info_tmpt .= "\n                <th>인쇄도수</th>"; //#0 prdt_dvs
        $mid_info_tmpt .= "\n                <td>%s</td>"; //#3 tmpt
        $mid_info_tmpt .= "\n            </tr>";

        $mid_mid .= sprintf($mid_info_tmpt, $tmpt);


    // 카다로그의 경우
    } else {

        $mid_info_base .= "\n            <tr>";
        $mid_info_base .= "\n                <th>재질(%s)</th>"; //#0 prdt_dvs
        $mid_info_base .= "\n                <td>%s</td>"; //#1 cate_paper
        $mid_info_base .= "\n            </tr>";
        $mid_info_base .= "\n            <tr>";
        $mid_info_base .= "\n                <th>인쇄도수(%s)</th>"; //#0 prdt_dvs
        $mid_info_base .= "\n                <td>%s</td>"; //#3 tmpt
        $mid_info_base .= "\n            </tr>";

        for ($i = 0; $i < $prdt_dvs_arr_count; $i++) {
            $prdt_dvs = $prdt_dvs_arr[$i];

            $paper = $paper_arr[$i];
            $page  = $page_arr[$i];
            $tmpt  = $tmpt_arr[$i];
            $after = $after_arr[$i];

            if (empty($paper)) {
                continue;
            }

            $mid_mid .= sprintf($mid_info_base, $prdt_dvs
                                              , $paper
                                              , $prdt_dvs
                                              , $tmpt);
        }

    }

    $mid_mid .= "\n           </tbody>";
    $mid_mid .= "\n       </table>";
    $mid_mid .= "\n   </td>";

    return $mid_top . $mid_mid;
}


/**
 * @brief 팝업 중단 html 생성
 *
 * @param $param = 정보
 *
 * @return 중단 html
 */
function getHtmlMid($param) {
    $common_cate_name = $param["common_cate_name"];
    $cate_name_arr = $param["cate_name_arr"];
    $size_arr      = $param["size_arr"];
    $amt_arr       = $param["amt_arr"];
    $amt_unit_arr  = $param["amt_unit_arr"];
    $count_arr     = $param["count_arr"];
    $paper_arr     = $param["paper_arr"];
    $tmpt_arr      = $param["tmpt_arr"];
    $after_arr     = $param["after_arr"];
    $pay_price     = $param["pay_price"];
    $supply_price  = $param["supply_price"];
    $tax           = $param["tax"];

    $prdt_count = count($cate_name_arr);

    $mid_top  = "\n    <tr>";
    $mid_top .= "\n        <td colspan=\"2\">";
    $mid_top .= "\n            <table class=\"inner_table_print_esti print_esti_price\">";
    $mid_top .= "\n                <caption style=\"font-weight:100; margin-bottom:6px;\">아래와 같이 견적합니다.</caption>";
    $mid_top .= "\n                <colgroup>";
    $mid_top .= "\n                    <col style=\"width:70%%;\">";
    $mid_top .= "\n                    <col style=\"\">";
    $mid_top .= "\n                </colgroup>";
    $mid_top .= "\n                <tbody>";
    $mid_top .= "\n                    <tr>";
    $mid_top .= "\n                        <th scope=\"border-right:none !important; text-align:left; font-weight:100;\">합계금액 \%s 원 + 부가세 \%s 원 + 배송비별도</th>"; //#0 합계금액, 부가세
    $mid_top .= "\n                        <th style=\"border-left:none !important; text-align:right; color:#000\">총 합계금액 : \%s</th>"; //#0 합계금액
    $mid_top .= "\n                    </tr>";
    $mid_top .= "\n                </tbody>";
    $mid_top .= "\n            </table>";
    $mid_top .= "\n        </td>";
    $mid_top .= "\n    </tr>";

    $mid_top = sprintf($mid_top, $supply_price
                               , $tax
                               , $pay_price);

    //$mid_top = sprintf($mid_top, $pay_price);

    $mid_mid_base  = "\n<tr>";
    $mid_mid_base .= "\n    <td style=\"vertical-align: top;\">";
    $mid_mid_base .= "\n        <table class=\"inner_table_print_esti\">";
    $mid_mid_base .= "\n            <colgroup>";
    $mid_mid_base .= "\n                <col style=\"width:35%%;\">";
    $mid_mid_base .= "\n                <col style=\"\">";
    $mid_mid_base .= "\n            </colgroup>";
    $mid_mid_base .= "\n            <tbody>";
    $mid_mid_base .= "\n                <tr>";
    $mid_mid_base .= "\n                    <th colspan=\"2\" class=\"table_title\">재질 및 규격</th>";
    $mid_mid_base .= "\n                </tr>";
    $mid_mid_base .= "\n                <tr>";
    $mid_mid_base .= "\n                    <th>품&nbsp;&nbsp;명</th>";
    $mid_mid_base .= "\n                    <td>%s</td>"; //#1 cate_name
    $mid_mid_base .= "\n                </tr>";
    $mid_mid_base .= "\n                <tr>";
    $mid_mid_base .= "\n                    <th>규격</th>";
    $mid_mid_base .= "\n                    <td>%s</td>"; //#2 cate_size
    $mid_mid_base .= "\n                </tr>";
    $mid_mid_base .= "\n                <tr>";
    $mid_mid_base .= "\n                    <th>수량</th>";
    $mid_mid_base .= "\n                    <td>%s%s &times; %s건</td>"; //#3 amt, amt_unit, count
    $mid_mid_base .= "\n                </tr>";    
    $mid_mid_base .= "\n                <tr>";
    $mid_mid_base .= "\n                    <th>재질</th>";
    $mid_mid_base .= "\n                    <td>%s</td>"; //#2 cate_paper
    $mid_mid_base .= "\n                </tr>";
    $mid_mid_base .= "\n                <tr>";
    $mid_mid_base .= "\n                    <th>인쇄도수</th>";
    $mid_mid_base .= "\n                    <td>%s</td>"; //#4 cate_tmpt
    $mid_mid_base .= "\n                </tr>";
    $mid_mid_base .= "\n            </tbody>";
    $mid_mid_base .= "\n        </table>";
    $mid_mid_base .= "\n    </td>";

    $mid_mid = '';
    for ($i = 0; $i < $prdt_count; $i++) {
        $cate_name = $cate_name_arr[$i];
        $paper     = $paper_arr[$i];
        $size      = $size_arr[$i];
        $tmpt      = $tmpt_arr[$i];
        $amt       = $amt_arr[$i];
        $amt_unit  = $amt_unit_arr[$i];
        $count     = $count_arr[$i];

        if (empty($common_cate_name) === false) {
            $cate_name = $common_cate_name . " - " . $cate_name;
        }

        $mid_mid .= sprintf($mid_mid_base, $cate_name
                                         , $size
                                         , $amt
                                         , $amt_unit
                                         , $count
                                         , $paper
                                         , $tmpt);
    }

    return $mid_top . $mid_mid;
}

/**
 * @brief 확정형 팝업 중단-하단 html 생성
 *
 * @param $param   = 정보
 * @param $aft_arr = 후공정 정보 배열
 *
 * @return 중단 html
 */
function getHtmlMidBot($param, $aft_arr) {
    $cate_name_arr = $param["cate_name_arr"];
    $count_arr     = $param["count_arr"];

    $paper_price  = $param["paper_price"];
    $print_price  = $param["print_price"];
    $output_price = $param["output_price"];
    $opt_price    = $param["opt_price"];
    $supply_price = $param["supply_price"];
    $tax          = $param["tax"];
    $sell_price   = $param["sell_price"];
    $sale_price   = $param["sale_price"];
    $pay_price    = $param["pay_price"];

    $after_html = '';
    foreach ($aft_arr as $aft_ko => $aft_en) {
        $after = $param[$aft_en . "_price"];

        if (empty($after) || $after === '0') {
            continue;
        }

        $after_html .= "\n            <tr>";
        $after_html .= "\n                <th>" . $aft_ko . "비</th>";
        $after_html .= "\n                <td>&#8361; " . $after . "</td>";
        $after_html .= "\n            </tr>";
    }

    $prdt_count = count($cate_name_arr);

    $mid_bot  = "\n<td style=\"vertical-align: top;\">";
    $mid_bot .= "\n    <table class=\"inner_table_print_esti detail_info\">";
    $mid_bot .= "\n        <colgroup>";
    $mid_bot .= "\n            <col style=\"width:25%%;\">";
    $mid_bot .= "\n            <col style=\"\">";
    $mid_bot .= "\n        </colgroup>";
    $mid_bot .= "\n        <tbody>";
    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th colspan=\"2\" class=\"table_title\">인쇄세부항목</th>";
    $mid_bot .= "\n            </tr>";
    if ($print_price !== '-') {
        $mid_bot .= "\n            <tr>";
        $mid_bot .= "\n                <th>인쇄비</th>";
        $mid_bot .= "\n                <td>&#8361; " . $print_price . "</td>"; //#8 print_price
        $mid_bot .= "\n            </tr>";
    }

    $mid_bot .= $after_html;

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>옵션비</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#10-1 opt_price
    $mid_bot .= "\n            </tr>";

    $mid_bot_count .= "\n            <tr>";
    $mid_bot_count .= "\n                <th>주문건수</th>";
    $mid_bot_count .= "\n                <td>%s건</td>"; //#11 count
    $mid_bot_count .= "\n            </tr>";
    for ($i = 0; $i < $prdt_count; $i++) {
        $cate_name = $cate_name_arr[$i];
        $count     = $count_arr[$i];

        $mid_bot .= sprintf($mid_bot_count, $count);
    }

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>공급가</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#12 supply_price
    $mid_bot .= "\n            </tr>";

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>부가세</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#13 tax
    $mid_bot .= "\n            </tr>";

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>정상판매가</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#14 sell_price
    $mid_bot .= "\n            </tr>";

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>할인금액</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#15 sale_price
    $mid_bot .= "\n            </tr>";

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>결제금액</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#16 pay_price
    $mid_bot .= "\n            </tr>";
    $mid_bot .= "\n        </tbody>";
    $mid_bot .= "\n    </table>";
    $mid_bot .= "\n </td>";
    $mid_bot .= "\n </tr>";

    $mid_bot  = sprintf($mid_bot, $opt_price
                                , $supply_price
                                , $tax
                                , $sell_price
                                , $sale_price
                                , $pay_price);

    return $mid_bot;
}

/**
 * @brief 책자형, 계산형 팝업 중단-하단 html 생성
 *
 * @param $param   = 정보
 * @param $aft_arr = 후공정 정보 배열
 *
 * @return 중단 html
 */
function getCalcHtmlMidBot($param, $aft_arr) {
    $cate_name_arr = $param["cate_name_arr"];
    $count_arr     = $param["count_arr"];

    $paper_price  = $param["paper_price"];
    $print_price  = $param["print_price"];
    $output_price = $param["output_price"];
    $opt_price    = $param["opt_price"];
    $supply_price = $param["supply_price"];
    $tax          = $param["tax"];
    $sell_price   = $param["sell_price"];
    $sale_price   = $param["sale_price"];
    $pay_price    = $param["pay_price"];

    $after_html = '';
    foreach ($aft_arr as $aft_ko => $aft_en) {
        $after = $param[$aft_en . "_price"];

        if (empty($after) || $after === '0') {
            continue;
        }

        $after_html .= "\n            <tr>";
        $after_html .= "\n                <th>" . $aft_ko . "비</th>";
        $after_html .= "\n                <td>&#8361; " . $after . "</td>";
        $after_html .= "\n            </tr>";
    }

    $prdt_count = count($cate_name_arr);

    $mid_bot  = "\n<td style=\"vertical-align: top;\">";
    $mid_bot .= "\n    <table class=\"inner_table_print_esti detail_info\">";
    $mid_bot .= "\n        <colgroup>";
    $mid_bot .= "\n            <col style=\"width:25%%;\">";
    $mid_bot .= "\n            <col style=\"\">";
    $mid_bot .= "\n        </colgroup>";
    $mid_bot .= "\n        <tbody>";
    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th colspan=\"2\" class=\"table_title\">인쇄세부항목</th>";
    $mid_bot .= "\n            </tr>";
    if ($paper_price !== '-') {
        $mid_bot .= "\n            <tr>";
        $mid_bot .= "\n                <th>종이비</th>";
        $mid_bot .= "\n                <td>&#8361; " . $paper_price . "</td>"; //#7 paper_price
        $mid_bot .= "\n            </tr>";
    }
    if ($output_price !== '-') {
        $mid_bot .= "\n            <tr>";
        $mid_bot .= "\n                <th>출력비</th>";
        $mid_bot .= "\n                <td>&#8361; " . $output_price . "</td>"; //#9 output_price
        $mid_bot .= "\n            </tr>";
    }
    if ($print_price !== '-') {
        $mid_bot .= "\n            <tr>";
        $mid_bot .= "\n                <th>인쇄비</th>";
        $mid_bot .= "\n                <td>&#8361; " . $print_price . "</td>"; //#8 print_price
        $mid_bot .= "\n            </tr>";
    }

    $mid_bot .= $after_html;

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>옵션비</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#10-1 opt_price
    $mid_bot .= "\n            </tr>";

    $mid_bot_count .= "\n            <tr>";
    $mid_bot_count .= "\n                <th>주문건수</th>";
    $mid_bot_count .= "\n                <td>%s건</td>"; //#11 count
    $mid_bot_count .= "\n            </tr>";
    for ($i = 0; $i < $prdt_count; $i++) {
        $cate_name = $cate_name_arr[$i];
        $count     = $count_arr[$i];

        $mid_bot .= sprintf($mid_bot_count, $count);
    }

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>공급가</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#12 supply_price
    $mid_bot .= "\n            </tr>";

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>부가세</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#13 tax
    $mid_bot .= "\n            </tr>";

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>정상판매가</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#14 sell_price
    $mid_bot .= "\n            </tr>";

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>할인금액</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#15 sale_price
    $mid_bot .= "\n            </tr>";

    $mid_bot .= "\n            <tr>";
    $mid_bot .= "\n                <th>결제금액</th>";
    $mid_bot .= "\n                <td>&#8361; %s</td>"; //#16 pay_price
    $mid_bot .= "\n            </tr>";
    $mid_bot .= "\n        </tbody>";
    $mid_bot .= "\n    </table>";
    $mid_bot .= "\n </td>";
    $mid_bot .= "\n </tr>";

    $mid_bot  = sprintf($mid_bot, $opt_price
                                , $supply_price
                                , $tax
                                , $sell_price
                                , $sale_price
                                , $pay_price);

    return $mid_bot;
}


/**
 * @brief 팝업 하단 html 생성
 *
 * @param $param   = 정보
 *
 * @return 하단 html
 */
function getHtmlAft($param) {

    $after_arr = $param["after_arr"];

    /*
    var_dump($after_arr);
    exit;
    */

    $html_bot  = "\n<tr>";
    $html_bot .= "\n<td colspan=\"2\" style=\"vertical-align: top;\">";
    $html_bot .= "\n    <table class=\"inner_table_print_esti\">";
    $html_bot .= "\n        <tbody>";
    $html_bot .= "\n            <tr>";
    $html_bot .= "\n                <th class=\"table_title\">후가공 세부내역</th>";
    $html_bot .= "\n            </tr>";
    $html_bot .= "\n            <tr>";
    $html_bot .= "\n                <td>";
    $html_bot .= "\n                    <table class=\"after_detail\">";
    $html_bot .= "\n                        <tbody>";

    $after_html = '';
    foreach ($after_arr as $val) {
        if (strpos($val,"후가공을") !== false) {
            $val = " ";
        }

        $after_html .= "\n            <tr>";
        $after_html .= "\n                <td>$val</td>";
        $after_html .= "\n            </tr>";
    }
    $html_bot .= $after_html;

    $html_bot .= "\n                        </tbody>";
    $html_bot .= "\n                    </table>";
    $html_bot .= "\n                </td>";
    $html_bot .= "\n            </tr>";
    $html_bot .= "\n        </tbody>";
    $html_bot .= "\n    </table>";
    $html_bot .= "\n </td>";
    $html_bot .= "\n </tr>";

    
    return $html_bot;
}

/**
 * @brief 팝업 비고 html 생성 (개발 필요)
 *
 * @param $param   = 정보
 *
 * @return 말단 html
 */
function getHtmlEtc($param) {

    $after_arr = $param["after_arr"];

    $html_etc  = "\n<tr>";
    $html_etc .= "\n<td colspan=\"2\" style=\"vertical-align: top;\">";
    $html_etc .= "\n    <table class=\"inner_table_print_esti\">";
    $html_etc .= "\n        <tbody>";
    $html_etc .= "\n            <tr>";
    $html_etc .= "\n                <th class=\"table_title\">비&nbsp;&nbsp;고</th>";
    $html_etc .= "\n            </tr>";
    $html_etc .= "\n            <tr>";
    $html_etc .= "\n                <td>";
    $html_etc .= "\n                    <table class=\"after_detail\">";
    $html_etc .= "\n                        <tbody>";

    $after_html = '';
    foreach ($after_arr as $aft_ko => $aft_en) {
        $after = $param[$aft_en . "_price"];

        if (empty($after) || $after === '0') {
            continue;
        }

        $after_html .= "\n            <tr>";
        $after_html .= "\n                <th>" . $aft_ko . "비</th>";
        $after_html .= "\n                <td>&#8361; " . $after . "</td>";
        $after_html .= "\n            </tr>";
    }

    $html_etc .= "\n                        </tbody>";
    $html_etc .= "\n                    </table>";
    $html_etc .= "\n                </td>";
    $html_etc .= "\n            </tr>";
    $html_etc .= "\n        </tbody>";
    $html_etc .= "\n    </table>";
    $html_etc .= "\n </td>";
    $html_etc .= "\n </tr>";

    
    return $html_etc;
}
?>
