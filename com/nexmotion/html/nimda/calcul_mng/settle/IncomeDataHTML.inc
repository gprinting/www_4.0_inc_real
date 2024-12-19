<?
/* 
 * 금전출납부 이체자료 list  생성 
 * $result : $result->fields["regi_date"] = "등록일자" 
 * $result : $result->fields["evid_date"] = "증빙일자" 
 * $result : $result->fields["depo_withdraw_path"] = "입출금경로" 
 * $result : $result->fields["depo_withdraw_path_detail"] = "입출금경로상세" 
 * $result : $result->fields["income_price"] = "수입 금액" 
 * $result : $result->fields["trsf_income_price"] = "이체 수입 금액" 
 * 
 * return : list
 */
function makeIncomeList($conn, $dao,$param, $result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    $ret = '<thead>';
	$ret .= '<tr>';
    $ret .= '<th class="bm2px">No.</th>';
    $ret .= '<th class="bm2px">채널</th>';
	$ret .= '<th class="bm2px">회원명</th>';
	$ret .= '<th class="bm2px">총매출</th>';
	$ret .= '<th class="bm2px">에누리</th>';
    $ret .= '<th class="bm2px">순매출</th>';
    $ret .= '<th class="bm2px">입금총계</th>';
    $ret .= '<th class="bm2px">선입금</th>';
    $ret .= '<th class="bm2px">가상계좌</th>';
    $ret .= '<th class="bm2px">카드선입금</th>';
    $ret .= '<th class="bm2px">카드결제</th>';
	$ret .= '<th class="bm2px">현금</th>';
    $ret .= '<th class="bm2px">기타입금</th>';
    $ret .= '<th class="bm2px">기타</th>';

    $j = 0;
    $start_date = date($param["date_from"]);
    if($param['show_day'] == "Y") {
        while (1) {
            $ret .= '<th class="bm2px">' . date('d일', strtotime($start_date)) . '</th>';

            $start_date = date('Y-m-d', strtotime("+1 day", strtotime($start_date)));
            if ($start_date > $param["date_to"]) {
                break;
            }
            $j++;
        }
    }


	$ret .= '</tr>';
	$ret .= '</thead>';

    while ($result && !$result->EOF) {
        //sales_price, adjust_price, depo_price
        $office_nick = $result->fields["member_name"];
        $update_date = $result->fields["update_date"];
        $member_seqno = $result->fields["member_seqno"];
        echo $result->fields["pay_price"] . " + " . $result->fields["card_pay_price"] . " - " . $result->fields["adjust_sales"];
        $sales_price = $result->fields["pay_price"] + $result->fields["card_pay_price"] - $result->fields["adjust_sales"];
        $adjust_price = $result->fields["enuri"]; //$result->fields["adjust_price"];

        $sell_channel = $result->fields["sell_channel"];
        $all_depo_price = $result->fields["depo_price"]
            + $result->fields["card_depo_price"]
            + $result->fields["card_pay_price"];
        $card_depo_price = $result->fields["card_depo_price"];
        $card_pay_price = $result->fields["card_pay_price"];

        $prepay_rs = $dao->selectPrepayPriceUntilDate($conn, $result->fields["id"], $param["date_to"]);
        $prepay_fields = $prepay_rs->fields;
        $prepay_price = doubleval($prepay_fields[0]);
        $cash_depo = $result->fields["cash_depo"];

        //if($adjust_price < 0) {
        //    $sales_price -= $adjust_price;
        //    $adjust_price = 0;
        //}
        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }

        $param1 = array();
        $param1["date_from"] = $param["date_from"];
        $param1["date_to"] = $param["date_to"];
        $param1["member_seqno"] = $result->fields["member_seqno"];

        $day_pay_price = "";
        $start_date = date($param["date_from"]);

        if($param['show_day'] == "Y") {
            $rs_byday = $dao->selectByDayIncomeList2($conn, $param1);
            while (1) {
                $is_exist = false;
                //
                while ($rs_byday && !$rs_byday->EOF) {
                    if ($start_date == $rs_byday->fields["date_range"]) {
                        $day_pay_price .= "<td style='text-align: right;'>" . number_format($rs_byday->fields["pay_price"] + $rs_byday->fields["card_pay_price"] + $rs_byday->fields["adjust_price"]) . "</td>";
                        $is_exist = true;
                        break;
                    }

                    $rs_byday->moveNext();
                }
                $rs_byday->moveFirst();
                if ($is_exist == false)
                    $day_pay_price .= "<td style='text-align: right;'>0</td>";
                $start_date = date('Y-m-d', strtotime("+1 day", strtotime($start_date)));
                if ($start_date > $param["date_to"]) {
                    break;
                }
            }
        }


        $list .= "\n    <td>%s</td>"; // 채널
        $list .= "\n    <td style='text-align: left;'>%s</td>"; // 채널
        $list .= "\n    <td style='text-align: left;'>%s</td>"; // 회원명
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 매출총액
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 조정총액
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 순매출총액
        //$list .= "\n    <td>%s</td>"; // 순매출총액
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 입금총계
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 선입금
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 입금총액(가상계좌)
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 입금총액(카드)
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 카드건별결제
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 현금
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 기타입금
        $list .= "\n    <td>"; // 입금내역보기
        $list .= "\n   <button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width60\" onclick=\"showWithdraw('%s');\">상세내역</button>";
        $list .= "\n   <button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width60\" onclick=\"showTotalByDay('%s');\">일별집계</button>";
        $list .= "\n    </td>"; // 입금내역보기
        $list .= $day_pay_price;
        $list .= "\n  </tr>";


        $sum = $sales_price - $adjust_price;
        $ret .= sprintf($list, $i, $sell_channel,
            $office_nick,
            number_format($sales_price),
            number_format($adjust_price),
            number_format($sales_price - $adjust_price),
            number_format($all_depo_price), // 입금총계
            number_format($prepay_price), // 현재 선입금
            number_format($result->fields["depo_price"]), // 가상계좌
            number_format($card_depo_price), // 카드선입금
            number_format($card_pay_price), // 카드건별결제
            number_format($cash_depo), // 현금
            number_format($result->fields["etc_deposit"]), // 기타입금
            $member_seqno, $member_seqno);

        $result->moveNext();
        $i++;
    }

    return $ret;
}

/* 
 * 금전출납부 이체자료 list  생성 2
 * $result : $result->fields["regi_date"] = "등록일자" 
 * $result : $result->fields["evid_date"] = "증빙일자" 
 * $result : $result->fields["depo_withdraw_path"] = "입출금경로" 
 * $result : $result->fields["depo_withdraw_path_detail"] = "입출금경로상세" 
 * $result : $result->fields["income_price"] = "수입 금액" 
 * $result : $result->fields["trsf_income_price"] = "이체 수입 금액" 
 * 
 * return : list
 */
function makeIncomeList2($conn, $dao,$param, $result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    $ret = '<thead>';
	$ret .= '<tr>';
    $ret .= '<th class="bm2px"></th>';
    $ret .= '<th class="bm2px">No.</th>';
    $ret .= '<th class="bm2px">채널</th>';
	$ret .= '<th class="bm2px">회원명</th>';
	$ret .= '<th class="bm2px">총매출</th>';
	$ret .= '<th class="bm2px">에누리</th>';
    $ret .= '<th class="bm2px">순매출</th>';
    $ret .= '<th class="bm2px">입금총계</th>';
    $ret .= '<th class="bm2px">선입금</th>';
    $ret .= '<th class="bm2px">가상계좌</th>';
    $ret .= '<th class="bm2px">카드선입금</th>';
    $ret .= '<th class="bm2px">카드결제</th>';
	$ret .= '<th class="bm2px">현금</th>';
    $ret .= '<th class="bm2px">기타입금</th>';
    $ret .= '<th class="bm2px">기타</th>';

    $j = 0;
    $start_date = date($param["date_from"]);
    if($param['show_day'] == "Y") {
        while (1) {
            $ret .= '<th class="bm2px">' . date('d일', strtotime($start_date)) . '</th>';

            $start_date = date('Y-m-d', strtotime("+1 day", strtotime($start_date)));
            if ($start_date > $param["date_to"]) {
                break;
            }
            $j++;
        }
    }


	$ret .= '</tr>';
	$ret .= '</thead>';

    while ($result && !$result->EOF) {
        //sales_price, adjust_price, depo_price
        $office_nick = $result->fields["member_name"];
        $update_date = $result->fields["update_date"];
        $member_seqno = $result->fields["member_seqno"];
        echo $result->fields["pay_price"] . " + " . $result->fields["card_pay_price"] . " - " . $result->fields["adjust_sales"];
        $sales_price = $result->fields["pay_price"] + $result->fields["card_pay_price"] - $result->fields["adjust_sales"];
        $adjust_price = $result->fields["enuri"]; //$result->fields["adjust_price"];

        $sell_channel = $result->fields["sell_channel"];
        $all_depo_price = $result->fields["depo_price"]
            + $result->fields["card_depo_price"]
            + $result->fields["card_pay_price"];
        $card_depo_price = $result->fields["card_depo_price"];
        $card_pay_price = $result->fields["card_pay_price"];

        $prepay_rs = $dao->selectPrepayPriceUntilDate($conn, $result->fields["id"], $param["date_to"]);
        $prepay_fields = $prepay_rs->fields;
        $prepay_price = doubleval($prepay_fields[0]);
        $cash_depo = $result->fields["cash_depo"];

        //if($adjust_price < 0) {
        //    $sales_price -= $adjust_price;
        //    $adjust_price = 0;
        //}
        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }

        $param1 = array();
        $param1["date_from"] = $param["date_from"];
        $param1["date_to"] = $param["date_to"];
        $param1["member_seqno"] = $result->fields["member_seqno"];

        $day_pay_price = "";
        $start_date = date($param["date_from"]);

        if($param['show_day'] == "Y") {
            $rs_byday = $dao->selectByDayIncomeList2($conn, $param1);
            while (1) {
                $is_exist = false;
                //
                while ($rs_byday && !$rs_byday->EOF) {
                    if ($start_date == $rs_byday->fields["date_range"]) {
                        $day_pay_price .= "<td style='text-align: right;'>" . number_format($rs_byday->fields["pay_price"] + $rs_byday->fields["card_pay_price"] + $rs_byday->fields["adjust_price"]) . "</td>";
                        $is_exist = true;
                        break;
                    }

                    $rs_byday->moveNext();
                }
                $rs_byday->moveFirst();
                if ($is_exist == false)
                    $day_pay_price .= "<td style='text-align: right;'>0</td>";
                $start_date = date('Y-m-d', strtotime("+1 day", strtotime($start_date)));
                if ($start_date > $param["date_to"]) {
                    break;
                }
            }
        }

        $list .= "\n    <td><input type='checkbox' name='chk_list' id='%s' /></td>"; // 체크박스
        $list .= "\n    <td>%s</td>"; // 채널
        $list .= "\n    <td style='text-align: left;'>%s</td>"; // 채널
        $list .= "\n    <td style='text-align: left;'>%s</td>"; // 회원명
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 매출총액
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 조정총액
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 순매출총액
        //$list .= "\n    <td>%s</td>"; // 순매출총액
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 입금총계
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 선입금
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 입금총액(가상계좌)
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 입금총액(카드)
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 카드건별결제
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 현금
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 기타입금
        $list .= "\n    <td>"; // 입금내역보기
        $list .= "\n   <button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width60\" onclick=\"showWithdraw('%s');\">상세내역</button>";
        $list .= "\n   <button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width60\" onclick=\"showTotalByDay('%s');\">일별집계</button>";
        $list .= "\n    </td>"; // 입금내역보기
        $list .= $day_pay_price;
        $list .= "\n  </tr>";


        $sum = $sales_price - $adjust_price;
        $ret .= sprintf($list,$result->fields["member_seqno"] , $i, $sell_channel,
            $office_nick,
            number_format($sales_price),
            number_format($adjust_price),
            number_format($sales_price - $adjust_price),
            number_format($all_depo_price), // 입금총계
            number_format($prepay_price), // 현재 선입금
            number_format($result->fields["depo_price"]), // 가상계좌
            number_format($card_depo_price), // 카드선입금
            number_format($card_pay_price), // 카드건별결제
            number_format($cash_depo), // 현금
            number_format($result->fields["etc_deposit"]), // 기타입금
            $member_seqno, $member_seqno);

        $result->moveNext();
        $i++;
    }

    return $ret;
}

function makeByWeekIncomeList($result, $start_date, $end_date) {

    $ret = "";
    $i = 0;

    while ($result && !$result->EOF) {
        //sales_price, adjust_price, depo_price
        $date_range = $result->fields["date_range"];

        $tmp_date_range = explode('~', $date_range);
        if($tmp_date_range[0] < $start_date)
            $tmp_date_range[0] = $start_date;

        if($tmp_date_range[1] > $end_date)
            $tmp_date_range[1] = $end_date;
        $date_range = $tmp_date_range[0] . "~" . $tmp_date_range[1];
        $pay_price = $result->fields["pay_price"] + $result->fields["card_pay_price"] - $result->fields["adjust_sales"] - $result->fields["enuri"];
        $card_depo_price = $result->fields["card_depo_price"];
        $cart_pay_price = $result->fields["cart_pay_price"];
        $adjust_price = $result->fields["adjust_price"];
        $depo_price = $result->fields["depo_price"] + $result->fields["card_depo_price"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }

        $list .= "\n    <td>%s</td>"; // 기간
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 총매출액
        $list .= "\n  </tr>";


        $ret .= sprintf($list, $date_range, number_format($pay_price) . "");

        $result->moveNext();
        $i++;
    }

    return $ret;
}

function makeByMonthIncomeList($result, $start_date, $end_date) {

    $ret = "";
    $i = 0;

    while ($result && !$result->EOF) {
        //sales_price, adjust_price, depo_price
        $date_range = explode('-', $result->fields["date_range"])[0] . '-' . explode('-', $result->fields["date_range"])[1];

        $pay_price = $result->fields["pay_price"] + $result->fields["card_pay_price"];
        $card_depo_price = $result->fields["card_depo_price"];
        $cart_pay_price = $result->fields["cart_pay_price"];
        $adjust_price = $result->fields["adjust_price"];
        $depo_price = $result->fields["depo_price"] + $result->fields["card_depo_price"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }

        $list .= "\n    <td>%s</td>"; // 기간
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 총매출액
        $list .= "\n  </tr>";


        $ret .= sprintf($list, $date_range, number_format($pay_price) . "");

        $result->moveNext();
        $i++;
    }

    return $ret;
}

function makeByYearIncomeList($result, $start_date, $end_date) {

    $ret = "";
    $i = 0;

    while ($result && !$result->EOF) {
        //sales_price, adjust_price, depo_price
        $date_range = explode('-', $result->fields["date_range"])[0];

        echo $date_range;
        $pay_price = $result->fields["pay_price"] + $result->fields["card_pay_price"];
        $card_depo_price = $result->fields["card_depo_price"];
        $cart_pay_price = $result->fields["cart_pay_price"];
        $adjust_price = $result->fields["adjust_price"];
        $depo_price = $result->fields["depo_price"] + $result->fields["card_depo_price"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }

        $list .= "\n    <td>%s</td>"; // 기간
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 총매출액
        $list .= "\n  </tr>";


        $ret .= sprintf($list, $date_range, number_format($pay_price) . "");

        $result->moveNext();
        $i++;
    }

    return $ret;
}

function makeMemberMemoList($result) {

    $ret = "";
    $i = 0;

    while ($result && !$result->EOF) {
        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }

        $list .= "\n    <td><input type='checkbox' id='memo_%s' name='member_memo' value='%s' /></td>"; // 체크박스
        $list .= "\n    <td style='width:50px;'>%s</td>"; // 기록일자
        $list .= "\n    <td>%s</td>"; // 메모
        $list .= "\n  </tr>";


        $ret .= sprintf($list,
            ($i + 1),
            $result->fields["member_memo_seqno"],
            $result->fields["insert_date"],
            $result->fields["memo"]);

        $result->moveNext();
        $i++;
    }

    return $ret;
}

function makeDayIncomeList($result) {

    $ret = "";
    $i = 0;

    while ($result && !$result->EOF) {
        //sales_price, adjust_price, depo_price
        $cont = $result->fields["a_cont"];
        $pay_price = $result->fields["pay_price"] + $result->fields["card_pay_price"] + $result->fields["adjust_price"] + $result->fields["depo_price"] + $result->fields["card_depo_price"];
        $order_detail = $result->fields["order_detail"];
        $pay_way = $result->fields["pay_way"];
        $card_cpn = $result->fields["card_cpn"];
        $card_num = $result->fields["card_num"];
        if($cont == "") {
            $cont = $result->fields["d_cont"];
            //$cont = $result->fields["order_detail"];
        }

        if($result->fields["adjust_price"] != 0) $order_detail = "에누리";
        if($result->fields["depo_price"] != 0 || $result->fields["card_depo_price"] != 0) $order_detail = "선입금";

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }

        $list .= "\n    <td>%s</td>"; // 인쇄제목
        $list .= "\n    <td>%s</td>"; // 수량
        $list .= "\n    <td>%s</td>"; // 지불방법
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 금액
        $list .= "\n    <td>%s</td>"; // 카드사
        $list .= "\n    <td>%s</td>"; // 카드번호
        //$list .= "\n    <td>%s</td>"; // 사이즈
        $list .= "\n  </tr>";


        $ret .= sprintf($list, $cont, $order_detail,$pay_way, number_format($pay_price) . "", $card_cpn, $card_num);

        $result->moveNext();
        $i++;
    }

    return $ret;
}

function makeByDayIncomeList($result, $dao, $conn) {

    $ret = "";
    $i = 0;

    $sum_pay_price = 0;
    $sum_adjust_price = 0;
    $sum_enuri = 0;
    $sum_pure_pay_price = 0;
    $sum_card_pay_price = 0;
    $sum_depo_price = 0;
    $origin_pay_price = 0;
    while ($result && !$result->EOF) {
        //sales_price, adjust_price, depo_price
        $date_range = $result->fields["date_range"];
        /*
        if($i == 0) {
            $prepay_rs = $dao->selectPrepayPriceUntilDate($conn, $result->fields["id"], $date_range);
            $fields = $prepay_rs->fields;
            $prepay_price  = intval($fields[0]);
        } else {
            $prepay_price = $prepay_price + $origin_pay_price - $depo_price - $calc_adjust_price;
        }
        */

        $prepay_rs = $dao->selectPrepayPriceUntilDate($conn, $result->fields["id"], $date_range);
        //$prepay_rs2 = $dao->selectPrepayPriceUntilDate2($conn, $result->fields["id"], $date_range);
        $fields = $prepay_rs->fields;
        $prepay_price  = intval($fields[0]);

        $pay_price = $origin_pay_price = $result->fields["pay_price"] + $result->fields["card_pay_price"] - $result->fields["adjust_sales"];
        $use_point = $result->fields["use_point"];
        $card_depo_price = $result->fields["card_depo_price"];
        $card_pay_price = $result->fields["card_pay_price"];
        $adjust_price = $calc_adjust_price = $result->fields["adjust_price"];
        $enuri = $result->fields["enuri"];
        //if($adjust_price < 0) $calc_adjust_price = 0;
        $depo_price = $result->fields["depo_price"] + $result->fields["card_depo_price"];
        //$prepay_rs = $dao->selectPrepayPriceUntilDate($conn, $result->fields["id"], $date_range);
        //$fields = $prepay_rs->fields;

        $sum_pay_price += $pay_price;
        $sum_enuri += $enuri;
        $sum_pure_pay_price += ($pay_price) - $enuri;
        $sum_card_pay_price += $card_pay_price;
        $sum_depo_price += $depo_price;

        if($adjust_price < 0) {
            //$sum_pay_price -= $sum_adjust_price;
            //$sum_adjust_price = 0;
        }

        //if($adjust_price < 0) {
        //    $pay_price -= $adjust_price;
        //    $sum_pay_price -= $adjust_price;
        //    $adjust_price = 0;
        //}

        if ($i%2 == 1) {
            $list  = "\n  <tr onclick='select_day_row(\"%s\", this)'>";
        } else {
            $list  = "\n  <tr class=\"cellbg\" onclick='select_day_row(\"%s\", this)'>";
        }

        $list .= "\n    <td>%s</td>"; // 기간
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 미수금
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 매출액
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 에누리
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 순매출액
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 포인트
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 카드매출
        $list .= "\n    <td style='text-align: right;'>%s</td>"; // 입금액
        $list .= "\n  </tr>";


        $ret .= sprintf($list, $date_range
            , $date_range
            , number_format($prepay_price) . ""
            , number_format($pay_price) . ""
            , number_format($enuri) . ""
            , number_format($pay_price - $enuri) . ""
            , number_format($use_point) . ""
            , number_format($card_pay_price) . ""
            , number_format($depo_price) . "");

        $result->moveNext();
        $i++;
    }

    $list  = "\n  <tr class=\"cellbg\">";
    $list .= "\n    <td>합계</td>"; // 기간
    $list .= "\n    <td style='text-align: right;'></td>"; // 미수금
    $list .= "\n    <td style='text-align: right;'>%s</td>"; // 매출액
    $list .= "\n    <td style='text-align: right;'>%s</td>"; // 에누리
    $list .= "\n    <td style='text-align: right;'>%s</td>"; // 순매출액
    $list .= "\n    <td style='text-align: right;'>%s</td>"; // 포인트
    $list .= "\n    <td style='text-align: right;'>%s</td>"; // 카드매출
    $list .= "\n    <td style='text-align: right;'>%s</td>"; // 입금액
    $list .= "\n  </tr>";


    $list = sprintf($list
        , number_format($sum_pay_price) . ""
        , number_format($sum_enuri) . ""
        , number_format($sum_pure_pay_price) . ""
        , number_format($use_point) . ""
        , number_format($sum_pure_pay_price) . ""
        , number_format($sum_depo_price) . "");

    $ret = $list . $ret;

    return $ret;
}
?>
