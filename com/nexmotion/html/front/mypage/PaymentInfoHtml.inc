<?
/*
 * 입출금 내역 리스트 생성
 *
 * return : list
 */
function makePaymentListHtml($rs, $util) {

    $list = "";
    $html  = "\n<tr>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    %s";
    $html .= "\n    <td class=\"left\">%s</td>";
    $html .= "\n    %s";
    $html .= "\n    %s";
    $html .= "\n</tr>";

    while ($rs && !$rs->EOF) {
        $deal_date       = substr($rs->fields["deal_date"], 0, 16);
        $dvs             = $rs->fields["dvs"];
        $cont            = $rs->fields["title"];
        $adjust_cont            = $rs->fields["cont"];
        $depo_price      = $rs->fields["depo_price"];
        $card_depo_price = $rs->fields["card_depo_price"];
        $pay_price       = $rs->fields["pay_price"];
        $adjust_price       = $rs->fields["adjust_price"];
        $order_state       = $rs->fields["order_state"];
        $input_typ       = $rs->fields["input_typ"];
        $com_price       = 0;

        if($adjust_price < 0) $pay_price += abs($adjust_price);
        if($adjust_price > 0) $depo_price += abs($adjust_price);
        if($adjust_price != 0) $cont = $rs->fields["cont"];

        // 선입금 충전
        if ($input_typ == '100') {
            if ($depo_price == 0) {
                $depo_price = "<td>-</td>";
            } else {
                $depo_price = "<td class=\"price plus\">" 
                    . number_format($depo_price) . "원</td>";
            }
            $com_price = $depo_price;

        // 선입금 카드충전
        } else if ($input_typ == "102") {
            if ($card_depo_price == 0) {
                $card_depo_price = "<td>-</td>";
            } else {
                $card_depo_price = "<td class=\"price plus\">" 
                    . number_format($card_depo_price) . "원</td>";
            }
            $com_price = $card_depo_price;
        } else if ($input_typ == '에누리') {
            $cont = $adjust_cont;
            if ($depo_price < 0) {
                $pay_price = "<td class=\"price minus\">"
                    . number_format($depo_price * -1) . "원</td>";
                $com_price = "<td class=\"price\">-</td>";
            } else {
                $com_price = "<td class=\"price plus\">"
                    . number_format($depo_price) . "원</td>";
                $pay_price = "<td class=\"price\">-</td>";
            }

            // 선입금 카드충전
        } else if ($input_typ == '매출') {
            $cont = $adjust_cont;
            if ($adjust_price < 0) {
                $pay_price = "<td class=\"price minus\">"
                    . number_format($adjust_price * -1) . "원</td>";
                $com_price = "<td class=\"price\">-</td>";
            } else {
                $com_price = "<td class=\"price plus\">"
                    . number_format($adjust_price) . "원</td>";
                $pay_price = "<td class=\"price\">-</td>";
            }

            // 선입금 카드충전
        } else if ($input_typ == '입금') {
            $cont = $adjust_cont;
            if ($depo_price < 0) {
                $pay_price = "<td class=\"price minus\">"
                    . number_format($depo_price * -1) . "원</td>";
                $com_price = "<td class=\"price\">-</td>";
            } else {
                $com_price = "<td class=\"price plus\">"
                    . number_format($depo_price) . "원</td>";
                $pay_price = "<td class=\"price\">-</td>";
            }
            if($cont == "") $cont = $rs->fields["cont2"];
            // 선입금 카드충전
        } else {
            $com_price = "<td class=\"price\">-</td>";
        }

        //if ($pay_price == 0) {
        //    $pay_price = "<td class=\"price\">-</td>";
        //} else {
        //    $pay_price = "<td class=\"price minus\">"
        //        . number_format($pay_price) . "원</td>";
        //}

        if ($dvs === "매출증가") {
            if($order_state != "1180") {
                $dvs = "<td><span class=\"status M\">" . "제품구입" . "</span></td>";
                $cont = "[" . $cont . "] 구입";
                $pay_price = "<td class=\"price minus\">"
                    . number_format($rs->fields["pay_price"]) . "원</td>";
            } else {
                $dvs = "<td><span class=\"status GRAY\">" . "구입취소" . "</span></td>";
                $com_price = "<td class=\"price plus\">"
                    . number_format($rs->fields["pay_price"]) . "원</td>";
                $cont = "[" . $cont . "] 구입취소";
                $pay_price = "<td class=\"price minus\">-</td>";
            }
        } else if ($dvs === "배송비") {
            if($order_state == "1180") {
                $dvs = "<td><span class=\"status GRAY\">" . "구입취소" . "</span></td>";
                $cont = "[" . $cont . "] 배송비 취소";
                $pay_price = "<td class=\"price minus\">-</td>";
                $com_price = "<td class=\"price plus\">"
                    . number_format($rs->fields["pay_price"]) . "원</td>";
                //$rs->moveNext();
                //continue;
            } else {
                $com_price = "<td class=\"price plus\">-</td>";
                $pay_price = "<td class=\"price minus\">"
                    . number_format($rs->fields["pay_price"]) . "원</td>";

                $dvs = "<td><span class=\"status M\">" . $dvs . "</span></td>";
                $cont = "[" . $cont . "] 배송비";
            }
        } else if ($dvs === "입금조정") {
            if($depo_price < 0)
                $color = "M";
            else {
                $color = "C";
            }

            $dvs = "<td><span class=\"status $color\">" . $dvs . "</span></td>";
            $cont = "[" . $cont . "]";
        } else {
            if($adjust_price < 0)
                $color = "M";
            else {
                $color = "C";
            }
            $dvs = "<td><span class=\"status $color\">" . $dvs . "</span></td>";
            $cont = $util->selectDepoInputType($input_typ);
        }



        $list .= sprintf($html, $deal_date, $dvs
                , $cont, $com_price
                , $pay_price);

        $rs->moveNext();
    }

    return $list;
}

/*
 * 거래 내역 리스트 생성
 *
 * return : list
 */
function makeTransactionalInfoHtml($rs) {

    $list = "";
    $html  = "\n<tr>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td class='left'>%s</td>";
    $html .= "\n    <td class='amount'>%s(%s)</td>";
    $html .= "\n    <td class='price'>&#8361;%s</td>";
    $html .= "\n    <td class='price discount'>&#8361;%s</td>";
    $html .= "\n    <td class='price'>&#8361;%s</td>";
    $html .= "\n</tr>";

    while ($rs && !$rs->EOF) {
        $deal_date = substr($rs->fields["deal_date"], 0, 10);
        $order_num = $rs->fields["order_num"];
        $order_detail = $rs->fields["order_detail"];

        if (!$rs->fields["amt"]) {
            $amt = "-";
        } else if ($rs->fields["amt"] == "-1" ) {
            $amt = $rs->fields["amt_unit_dvs"];
        } else {
            $amt = number_format($rs->fields["amt"]) . $rs->fields["amt_unit_dvs"];
        }

        if ( $rs->fields["count"] > 0 ) {
            $count = number_format($rs->fields["count"]) . "건";
        } else {
            $count = "";
        }

        if($rs->fields["dvs"] == "배송비") {
            $order_detail = "배송비";
            $amt = "";
            $count = "1건";
        }

        $sell_price = number_format($rs->fields["sell_price"]);
        $sale_price = number_format($rs->fields["sale_price"]);
        $pay_price = number_format($rs->fields["pay_price"] + $rs->fields["card_pay_price"]);

        $list .= sprintf($html, $deal_date
                , $order_num, $order_detail
                , $amt, $count
                , $sell_price, $sale_price
                , $pay_price);

        $rs->moveNext();
    }

    return $list;
}

/*
 * 거래 내역 리스트 생성
 *
 * return : list
 */
function makeTransactionPrintHtml($rs) {

    $list = "";
    $html .= "\n        <tr>";
    $html .= "\n            <td>%d</td>";
    $html .= "\n            <td>%s</td>";
    $html .= "\n            <td class=\"table_left\">%s</td>";
    $html .= "\n            <td class=\"table_left\">%s</td>";
    $html .= "\n            <td>%s</td>";
    $html .= "\n            <td>%s</td>";
    $html .= "\n            <td>&#8361;%s</td>";
    $html .= "\n            <td>&#8361;%s</td>";
    $html .= "\n            <td>&#8361;%s</td>";
    $html .= "\n            <td>&#8361;%s</td>";
    $html .= "\n            <td>&#8361;%s</td>";
    $html .= "\n        </tr>";

    $i = 1;
    while ($rs && !$rs->EOF) {
        $deal_date = substr($rs->fields["deal_date"], 5, 5);
        $title = $rs->fields["title"];
        $order_detail = $rs->fields["order_detail"];

        if (!$rs->fields["amt"]) {
            $amt = "-";
        } else if ($rs->fields["amt"] == "-1" ) {
            $amt = $rs->fields["amt_unit_dvs"]; 
        } else {
            $amt = number_format($rs->fields["amt"]) . $rs->fields["amt_unit_dvs"];
        }

        if ( $rs->fields["count"] > 0 ) {
            $count = number_format($rs->fields["count"]);
        } else {
            $count = "";
        }

        if($rs->fields["dvs"] == "배송비") {
            $order_detail = "배송비";
            $amt = "";
            $count = "1건";
        }

        $sell_price = number_format($rs->fields["sell_price"]);
        $adjust_price = number_format($rs->fields["adjust_price"]);
        $pay_price = number_format($rs->fields["pay_price"]);
        $depo_price = number_format($rs->fields["depo_price"]);
        $card_depo_price = number_format($rs->fields["card_depo_price"]);

        $list .= sprintf($html, $i, $deal_date
                , $title
                , $order_detail
                , $amt, $count
                , $sell_price, $adjust_price
                , $pay_price, $depo_price, $card_depo_price);

        $rs->moveNext();
        $i++;
    }

    return $list;
}

/*
 * 거래 내역 리스트 생성
 *
 * return : list
 */
function makeTransactionPrintHtml2($rs) {

    $list = "";
    $html .= "\n        <tr>";
    $html .= "\n            <td>%s</td>";
    $html .= "\n            <td>%s</td>";
    $html .= "\n            <td class=\"table_left\">%s</td>";
    $html .= "\n            <td class=\"table_left\">%s</td>";
    $html .= "\n            <td>%s</td>";
    $html .= "\n            <td>%s</td>";
    $html .= "\n            <td>&#8361;%s</td>";
    $html .= "\n            <td>&#8361;%s</td>";
    $html .= "\n            <td>&#8361;%s</td>";
    $html .= "\n            <td>&#8361;%s</td>";
    $html .= "\n            <td>&#8361;%s</td>";
    $html .= "\n        </tr>";

    $i = 1;
    while ($rs && !$rs->EOF) {
        $deal_date = substr($rs->fields["deal_date"], 5, 5);
        $title = $rs->fields["title"];
        $order_detail = $rs->fields["order_detail"];
        $cont = $rs->fields["cont"];

        if (!$rs->fields["amt"]) {
            $amt = "-";
        } else if ($rs->fields["amt"] == "-1" ) {
            $amt = $rs->fields["amt_unit_dvs"]; 
        } else {
            $amt = number_format($rs->fields["amt"]) . $rs->fields["amt_unit_dvs"];
        }

        if ( $rs->fields["count"] > 0 ) {
            $count = number_format($rs->fields["count"]);
        } else {
            $count = "";
        }

        if($rs->fields["dvs"] == "배송비") {
            $order_detail = "배송비";
            $amt = "";
            $count = "1건";
        }

        $sell_price = $rs->fields["sell_price"];
        $adjust_price = number_format($rs->fields["adjust_price"]);
        $pay_price = number_format($rs->fields["pay_price"]);
        $depo_price = number_format($rs->fields["depo_price"]);
        $card_depo_price = number_format($rs->fields["card_depo_price"]);

       if($sell_price < 0){
            $sell_price =  abs($sell_price);
        }else{
            $sell_price =  $sell_price * (-1);
        } 

        $list .= sprintf($html, "", $deal_date
                , $cont
                , $order_detail
                , $amt, 1
                , number_format($sell_price), $adjust_price
                , $pay_price, $depo_price, $card_depo_price);

        $rs->moveNext();
        $i++;
    }

    return $list;
}

/*
 * 거래 내역 리스트 생성
 *
 * return : list
 */
function makeTransactionPriceHtml($p_rs, $d_rs, $b_rs) {

    $sell_price = $p_rs->fields["sell_price"];
    $adjust_price = $p_rs->fields["adjust_price"];
    $pay_price = $p_rs->fields["pay_price"];
    $depo_price = $d_rs->fields["depo_price"];
    $card_depo_price = $d_rs->fields["card_depo_price"];
    $prepay_bal = $b_rs->fields["prepay_bal"];
    $sell_price2 = $p_rs->fields["sell_price2"];

    if($sell_price2 < 0){
        $sell_price2 =  abs($sell_price2);
    }else{
        $sell_price2 =  $sell_price2 * (-1);
    }


    $html  = "";
    $html .= "\n    <caption class=\"table_right\">";
    $html .= "\n        총매출액 : &#8361;" . number_format($pay_price+$sell_price2) . "원";
    $html .= "/ 에누리 : &#8361;" . number_format($adjust_price) . "원";
    $html .= "/ 순매출액 : &#8361;" . number_format(($pay_price+$sell_price2)-$adjust_price) . "원";
    $html .= "/ 입금액(가상계좌) : &#8361;" . number_format($depo_price) . "원";
    $html .= "/ 입금액(카드) : &#8361;" . number_format($card_depo_price) . "원";
   // $html .= "/ 일일잔액 : &#8361;" . number_format($prepay_bal) . "원";
    $html .= "\n    </caption>";

    return $html;
}

?>
