<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . '/common_lib/CommonUtil.inc');
/**
 * @brief 매출 대기리스트 HTML
 */
function makePublicStandByListHtml($conn, $dao, $rs, $param) {
  
    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr>";
    $html .= "\n    <td><input type='checkbox' class='check_box' name='standby_chk' value='%d'></td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\"><a href=\"#\"";
    $html .= " onclick=\"publicLayer('basic',";
    $html .= " %d,'%s','%s','%s','%s','%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '')\">";
    $html .= "%s</a></td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\"><input type='text' name='change_price' id='cp_%s' value=''> 
                  <button type='button' onclick='ChangePrice(%s);' data-seq='' class='btn_Turquoise01 btn_pu fix_height20 fix_width60'>
                        변경
                </button></td>";
    $html .= "\n    <td><select id='dvs_detail_%s' name='dvs_detail' class='fix_width100'>
							    <option value='세금계산서'>세금계산서</option>
							    <option value='지출증빙'>지출증빙</option>
                                <option value='소득공제'>소득공제</option>
                                <option value='미발행'>미발행</option>
                                <option value='카드건별'>카드건별</option>
							</select><button type=\"button\" onclick='StandbyChangeDvsPeriod(%d, \"%s\", \"%s\",\"%s\",\"%s\",\"%s\");' class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">변경</button></td>";
    $html .= "\n    <td><button type=\"button\" onclick=\"saveStateSeq(%d, '완료')\" class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">발행완료</button><button type=\"button\" onclick=\"showTaxReadyCasePopup(%d, '%s','%s', '대기')\" class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">건별보기</button></td>";
    $html .= "\n  </tr>";
    $i = 1 + $param["s_num"];
    $j = 1;
    $prev_channel = "";
    while ($rs && !$rs->EOF) {
        $param["member_seqno"] = $rs->fields["member_seqno"];
        $rs2 = $dao->selectAllPayprice2($conn, $param);
        $object_price = $rs->fields["pay_price"] - $rs->fields["enuri"] - $rs->fields["adjust_sales"];


       // $rs3 = $dao->selectTaxChkPrice($conn, $param);
       // $arr3 = (array) $rs3;
        
        // 20231129 세금계산서 수정 발급을 위한 체크 
        if($rs->fields['change_price'] != 0 ) {
            $object_price = $rs->fields['change_price'];
        }   

        if($object_price < 0) $object_price = 0;
        if($prev_channel != "" && ($rs->fields["sell_channel"] != $prev_channel))
            $i = 1;
        $prev_channel = $rs->fields["sell_channel"];
        $rs_html .= sprintf($html,
            $rs->fields["member_seqno"],
                $i,
                $rs->fields["sell_channel"],
                $rs->fields["member_name"],
                $rs->fields["crn"],
                $rs->fields["public_admin_seqno"],
                $rs->fields["public_date"],
                $rs->fields["cpn_admin_seqno"],
                $rs->fields["pay_year"] . "년 " .
                $rs->fields["pay_mon"] . "월",
                number_format($rs->fields["pay_price"]),
                number_format($rs->fields["pay_price"]),
                number_format($rs->fields["card_pay_price"]),
                number_format($rs->fields["adjust_price"]),
                $rs->fields["oa"],
                $rs->fields["before_oa"],
                number_format($object_price),
                $rs->fields["corp_name"],
                $rs->fields["repre_name"],
                $rs->fields["crn"],
                $rs->fields["bc"],
                $rs->fields["tob"],
                $rs->fields["addr"],
                $rs->fields["zipcode"],
                $rs->fields["req_mon"],
                $param["day"],
                number_format($rs->fields["unitprice"]),
                number_format($rs->fields["supply_price"]),
                number_format($rs->fields["vat"]),
                $rs->fields["corp_name"],
                $rs->fields["tel_num"],
                $rs->fields["cell_num"],
                number_format($rs2->fields["card_pay_price"] + $rs2->fields["pay_price"] - $rs2->fields["adjust_sales"]),
                number_format($rs2->fields["enuri"]),
                number_format($rs2->fields["card_pay_price"] + $rs2->fields["pay_price"] - $rs2->fields["adjust_sales"] - $rs2->fields["enuri"]),
                number_format($object_price),
                $rs->fields["member_seqno"],
                $rs->fields["member_seqno"],
                $j,
                $rs->fields["member_seqno"],
                $rs->fields["public_dvs"],
                $rs->fields["dvs_detail"],
                $param["year"],
                $param["mon"],
                $j,
                $rs->fields["member_seqno"],
                $rs->fields["member_seqno"],
                $param["year"],
                $param["mon"]
                );

        $i++;
        $j++;
        $rs->moveNext();
    }

    return $rs_html;
}

function makePublicAllListHtml($conn, $dao, $rs, $param) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr>";
    $html .= "\n    <td><input type='checkbox' class='check_box' name='standby_chk' value='%d'></td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n  </tr>";
    $i = 1 + $param["s_num"];
    $j = 1;
    $z = -1;
    $member_seqno = 0;

    $prev_channel = "";
    $is_new = false;
    $input_param = array();
    $z = -1;
    $member_seqno = 0;

    $year = $param["year"];
    $mon = $param["mon"];

    while($rs && !$rs->EOF) {
        $fields = $rs->fields;
        //셀 병합
        //$sheet->mergeCells('C'.$i.':E'.$i);
        //$sheet->mergeCells('F'.$i.':K'.$i);
        //엑셀 데이터 바디부분
        //$input_param["seq"] = $i-$j;
        $fields["deal_date"] = $year . "-" . $mon;
        if($member_seqno != $fields["member_seqno"]) {
            $all_pay_price = 0; // 매출금액
            $total_depo_price = 0; // 선입금충전(가상계좌)
            $total_card_depo_price = 0; // 선입금충전(카드)
            $total_account_all_pay_price = 0; // 선입금으로 결제한 금액 V
            $total_account_not_issued_pay_price = 0; // 가상계좌로 결제한 금액(아직 미발행) V
            $total_account_issued_pay_price = 0; // 가상계좌로 결제한 금액(발행완료) V
            $total_account_no_issue_pay_price = 0; // 미발행 V
            $total_cash_receipt_pay_price = 0; // 현금영수증 V
            $total_card_pay_price = 0; //카드(홈페이지) V
            $total_card_direct_pay_price = 0; //카드(방문) V
            $adjust_etc = 0;
            $adjust_price = 0;
            $adjust_sales = 0;
            $adjust_deposit = 0;
            $enuri = 0;
            $z++;

            $member_seqno = $fields["member_seqno"];
        }

        $sell_channel = $rs->fields["sell_channel"];
        $public_dvs = $rs->fields["public_dvs"];
        $public_state = $rs->fields["public_state"];
        $total_account_all_pay_price += $rs->fields["pay_price"];
        $adjust_price += $rs->fields["adjust_price"]; // 조정 에누리
        $enuri += $rs->fields["enuri"]; // 조정 에누리
        $adjust_sales -= $rs->fields["adjust_sales"]; // 조정 매출
        $adjust_deposit += $rs->fields["adjust_deposit"]; // 조정 입금
        $adjust_etc += $rs->fields["adjust_etc"];

        $total_card_direct_pay_price += $rs->fields["card_depo_price1"];

        if($public_dvs == "세금계산서") {
            if($public_state == "대기")
                $total_account_not_issued_pay_price = $rs->fields["pay_price"];
            if($public_state == "완료")
                $total_account_issued_pay_price = $rs->fields["pay_price"];
        }

        if($public_dvs == "미발행") {
            $total_account_no_issue_pay_price += $rs->fields["pay_price"];
            $total_card_pay_price += $rs->fields["card_pay_price"]; //카드(홈페이지)
        }

        if($public_dvs == "소득공제" || $public_dvs == "지출증빙") {
            $total_cash_receipt_pay_price += $rs->fields["pay_price"];
        }

        if($public_dvs == "카드건별") {
            $total_account_issued_pay_price += $rs->fields["pay_price"];
        }

        $total_depo_price += $rs->fields["depo_price"];
        $total_card_depo_price += $rs->fields["card_depo_price2"];


        $input_param[$z]["sell_channel"] = $sell_channel;
        $input_param[$z]["deal_date"] = $year . "-" . $mon; // 신청일자
        $input_param[$z]["issued_date"] = $year . "-" . $mon; // 발급일자
        $input_param[$z]["member_name"] = $fields["member_name"]; // 업체명
        $input_param[$z]["corp_name"] = $fields["corp_name"]; // 사업자 상호
        $input_param[$z]["crn"] = $fields["crn"];  // 사업자번호
        $input_param[$z]["tel_num"] = $fields["tel_num"];  // 전화번호
        $input_param[$z]["cell_num"] = $fields["cell_num"];  // 휴대폰번호
        $input_param[$z]["all_pay_price"] = // 총 매출금액 - F
            $total_account_not_issued_pay_price  // 가상계좌 매출 V
            + $total_account_no_issue_pay_price  // 현금(미발행) V
            + $total_cash_receipt_pay_price  // 현금영수증 V
            + $total_account_issued_pay_price // 세금계산서 선발행금액 V
            + $total_card_direct_pay_price // 카드(단말기) V
            + $total_card_pay_price // 카드건별결제(웹) V
            + $adjust_sales; // 매출조정금액 V
        $input_param[$z]["adjust_price"] = $adjust_price; // 기타 - G
        $input_param[$z]["adjust_sales"] = $adjust_sales; // 기타 - G
        $input_param[$z]["adjust_deposit"] = $adjust_deposit; // 기타 - G
        $input_param[$z]["adjust_etc"] = $adjust_etc; // 기타 - G
        $input_param[$z]["enuri"] = $enuri; // 기타 - G
        $input_param[$z]["pure_all_pay_price"] = $input_param[$z]["all_pay_price"] - $enuri; // 기타 - G
        // H = F - G
        $input_param[$z]["card_depo_price"] = $total_card_depo_price; // 카드 충전금 - I
        $input_param[$z]["depo_price"] = $total_depo_price - $adjust_etc; // 가상계좌 충전금 - J
        $input_param[$z]["cash_price_issue"] = $rs->fields["adjust_etc"]; // 현금결제 - K
        $input_param[$z]["total_cash_receipt_pay_price"] = $total_cash_receipt_pay_price; // 현금영수증 - L
        $input_param[$z]["total_account_issued_pay_price"] = $total_account_issued_pay_price; // 세금계산서 선발행금액 - M
        $input_param[$z]["total_account_no_issue_pay_price"] = $total_account_no_issue_pay_price; // 가상계좌(미발행요청) - N
        $input_param[$z]["total_account_not_issued_pay_price"] = $total_account_not_issued_pay_price; // 가상계좌(아직 미발행)
        $input_param[$z]["total_card_pay_price"] = $total_card_pay_price; // 카드건별결제(웹) - O
        $input_param[$z]["total_card_direct_pay_price"] = $total_card_direct_pay_price; // 카드(단말기) - P
        $input_param[$z]["cash_price_no_issue"] = 0; // 현금결제 - Q
        $input_param[$z]["issue_price"] = $input_param[$z]["pure_all_pay_price"]
            - $total_cash_receipt_pay_price // M - 현금영수증
            - $total_account_no_issue_pay_price // N -
            - $total_card_pay_price // O
            - $total_card_direct_pay_price // P
            - $total_card_direct_pay_price // Q - 카드(단말기)
            - 0;// R

        $param["member_seqno"] = $rs->fields["member_seqno"];
        $rs1 = $dao->selectPublicStandByList($conn, $param);
        $issue_price = $rs1->fields["pay_price"] - $rs1->fields["enuri"] - $rs1->fields["adjust_sales"];
        $input_param[$z]["issue_price"] = $issue_price;
        if($input_param[$z]["issue_price"] < 0) $input_param[$z]["issue_price"] = 0;
        if($member_seqno == "1994") {
            //echo $input_param[$z]["pure_all_pay_price"] . "|" . $adjust_price. "|";
        }

        //$input_param[$z]["memo"] = $dao->selectPackageCount($conn, $fields["bun_dlvr_order_num"], $fields["order_common_seqno"], $dlvr_dvs, $fields["member_seqno"]);
        //$rs2 = $dao->selectOrdererInfo($conn, $fields["order_common_seqno"]);
        $rs->MoveNext();
    }


    $i = 1;
    $ret_all_pay_price = 0;
    $ret_enuri = 0;
    $ret_pure_all_pay_price = 0;
    $ret_card_pay_price = 0;
    foreach ($input_param as $param) {
        $rs_html .= sprintf($html,
            $i,
            $i++,
            $param["sell_channel"],
            $param["deal_date"],
            $param["issued_date"],
            $param["member_name"],
            $param["corp_name"],
            $param["crn"],
            $param["tel_num"],
            $param["cell_num"],
            number_format($param["all_pay_price"]),
            number_format($param["enuri"]),
            number_format($param["pure_all_pay_price"]),
            number_format($param["card_depo_price"]),
            number_format($param["depo_price"]),
            number_format($param["adjust_deposit"]),
            number_format($param["adjust_etc"]),
            number_format($param["total_cash_receipt_pay_price"]),
            number_format($param["total_account_issued_pay_price"]),
            number_format($param["total_account_no_issue_pay_price"]),
            number_format($param["total_card_pay_price"]),
            number_format($param["total_card_direct_pay_price"]),
            number_format($param["cash_price_no_issue"]),
            number_format($param["issue_price"]),
        );

        $ret_all_pay_price += $param["all_pay_price"];
        $ret_enuri += $param["enuri"];
        $ret_pure_all_pay_price += $param["pure_all_pay_price"];
        $ret_card_pay_price += $param["card_depo_price"] + $param["total_card_pay_price"] + $param["total_card_direct_pay_price"];
    }

    $ret = array();
    $ret["html"] = $rs_html;
    $ret["all_pay_price"] = $ret_all_pay_price;
    $ret["enuri"] = $ret_enuri;
    $ret["pure_all_pay_price"] = $ret_pure_all_pay_price;
    $ret["all_card_pay_price"] = $ret_card_pay_price;
    return $ret;
}



function makeCardpayListHtml($conn, $dao, $rs, $param) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr>";
    $html .= "\n    <td><input type='checkbox' class='check_box' name='standby_chk' value='%d'></td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\"><a href=\"#\"";
    $html .= " onclick=\"publicLayer('basic',";
    $html .= " %d,'%s','%s','%s','%s','%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '')\">";
    $html .= "%s</a></td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td><button type=\"button\" onclick=\"showCardpayListPopup(%d, '%s','%s')\" class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">건별보기</button></td>";
    $html .= "\n  </tr>";
    $i = 1 + $param["s_num"];

    $prev_channel = "";
    $param["kind"] = "card or cash";
    while ($rs && !$rs->EOF) {
        $param["member_seqno"] = $rs->fields["member_seqno"];
        $rs2 = $dao->selectAllPayprice2($conn, $param);
        if($prev_channel != "" && ($rs->fields["sell_channel"] != $prev_channel))
            $i = 1;
        $prev_channel = $rs->fields["sell_channel"];

        $object_price = ($rs->fields["pay_price"] + $rs->fields["card_pay_price"]) - $rs->fields["adjust_sales"] - $rs->fields["enuri"];
        if($object_price < 0) $object_price = 0;
        $rs_html .= sprintf($html,
            $rs->fields["public_admin_seqno"],
            $i,
            $rs->fields["sell_channel"],
            $rs->fields["member_name"],
            $rs->fields["crn"],
            $rs->fields["public_admin_seqno"],
            $rs->fields["public_date"],
            $rs->fields["cpn_admin_seqno"],
            $rs->fields["pay_year"] . "년 " .
            $rs->fields["pay_mon"] . "월",
            number_format($rs->fields["card_pay_price"]),
            number_format($rs->fields["card_pay_price"]),
            number_format($rs->fields["card_pay_price"]),
            number_format($rs->fields["adjust_price"]),
            $rs->fields["oa"],
            $rs->fields["before_oa"],
            number_format($object_price),
            $rs->fields["corp_name"],
            $rs->fields["repre_name"],
            $rs->fields["crn"],
            $rs->fields["bc"],
            $rs->fields["tob"],
            $rs->fields["addr"],
            $rs->fields["zipcode"],
            $rs->fields["req_mon"],
            $param["day"],
            number_format($rs->fields["unitprice"]),
            number_format($rs->fields["supply_price"]),
            number_format($rs->fields["vat"]),
            $rs->fields["corp_name"],
            $rs->fields["tel_num"],
            $rs->fields["cell_num"],
            number_format($rs2->fields["card_pay_price"] + $rs2->fields["pay_price"] - $rs2->fields["adjust_sales"]),  // 정식회원명
            number_format($rs2->fields["enuri"]),  // 정식회원명
            number_format($rs2->fields["card_pay_price"] + $rs2->fields["pay_price"] - $rs2->fields["adjust_sales"] - $rs2->fields["enuri"]),  // 정식회원명
            number_format($rs->fields["card_depo_price"] + $rs->fields["card_pay_price"]),
            $rs->fields["member_seqno"],
            $param["year"],
            $param["mon"]
        );

        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}



/**
 * @brief 매출 현금영수증리스트 HTML
 */
function makeCashreceiptListHtml($conn, $dao, $rs, $param) {
  
    if (!$rs) {
        return false;
    }

    if($param['state'] == "대기") {
        $display_change = "";
        $display_ready = "";
        $display_complete = "style=\"display: none;\"";
    } else if($param['state'] == "완료") {
        $display_change = "style=\"display: none;\"";
        $display_ready = "style=\"display: none;\"";
        $display_complete = "";
    }

    $rs_html = "";
    $html  = "\n  <tr>";
    $html .= "\n    <td><input type='checkbox' class='check_box' name='cashreceipt_chk' value='%d'></td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td><select id='dvs_detail_%s' name='dvs_detail' class='fix_width100' %s>
							    <option value='세금계산서'>세금계산서</option>
							    <option value='지출증빙'>지출증빙</option>
                                <option value='소득공제'>소득공제</option>
                                <option value='미발행'>미발행</option>
                                <option value='카드건별'>카드건별</option>
							</select><button type=\"button\" %s onclick='changeDvsPeriod(%d, \"%s\", \"%s\",\"%s\",\"%s\",\"%s\");' class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">변경</button></td>";
    $html .= "\n    <td><button type=\"button\" %s onclick='saveCashreceipt(%d, \"%s\",\"%s\",\"%s\")' class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">발행완료</button><button type=\"button\" %s onclick='cancelCashreceipt(%d, \"%s\",\"%s\",\"%s\")' class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">발행취소</button><button type=\"button\" onclick=\"showReceiptReadyCasePopup(%d, '%s','%s', '%s','%s')\" class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">건별보기</button></td>";
    //$html .= "\n   <button type=\"button\" onclick=\"changeDvs(%d, '미발행')\" class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">미발행</button>";
    $html .= "\n  </tr>";
    $i = 1 + $param["s_num"];
    $j = 1;
    $prev_channel = "";
    while ($rs && !$rs->EOF) {
        $param["member_seqno"] = $rs->fields["member_seqno"];
        $rs2 = $dao->selectAllPayprice2($conn, $param);
        $object_price = $rs->fields["pay_price"] - $rs->fields["adjust_sales"] - $rs->fields["enuri"];
        if($prev_channel != "" && ($rs->fields["sell_channel"] != $prev_channel))
            $i = 1;
        $prev_channel = $rs->fields["sell_channel"];

        $hidden = "";
        if($rs->fields["public_state"] == "완료") {
            $hidden = "style=\"display: none;\"";
        }
        $rs_html .= sprintf($html,
                $rs->fields["member_pay_history_seqno"],
                $i,
                $rs->fields["sell_channel"],
                $rs->fields["member_name"],
                $rs->fields["public_dvs"],
                $rs->fields["dvs_detail"],
                $rs->fields["corp_name"] == null ? "미등록" : $rs->fields["corp_name"],
                $rs->fields["tel_num"],
                $rs->fields["cell_num"],
            number_format($rs2->fields["card_pay_price"] + $rs2->fields["pay_price"] - $rs2->fields["adjust_sales"]),
            number_format($rs2->fields["enuri"]),
            number_format($rs2->fields["card_pay_price"] + $rs2->fields["pay_price"] - $rs2->fields["adjust_sales"] - $rs2->fields["enuri"]),
            number_format($object_price),
                $j,
                $display_change,
                $display_change,
                $rs->fields["member_seqno"],
                $rs->fields["public_dvs"],
                $rs->fields["dvs_detail"],
                $param["year"],
                $param["mon"],
                $j,
                $display_ready,
                $rs->fields["member_seqno"],
                $param["year"],
                $param["mon"],
                $rs->fields["dvs_detail"],
                $display_complete,
                $rs->fields["member_seqno"],
                $param["year"],
                $param["mon"],
            $rs->fields["dvs_detail"],
                $rs->fields["member_seqno"],
                $param["year"],
                $param["mon"],
                $rs->fields["public_state"],
                $rs->fields["dvs_detail"],
                //$hidden,
                $rs->fields["member_pay_history_seqno"]
                );

        $i++;
        $j++;
        $rs->moveNext();
    }

    return $rs_html;
}

/**
 * @brief 매출 미발행(현금순매출)리스트 HTML
 */
function makeUnissuedListHtml($conn, $dao, $rs, $param) {
  
    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr>";
    $html .= "\n    <td><input type='checkbox' class='check_box' name='unissued_chk' value='%d'></td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td><select id='unissue_dvs_detail_%s' name='unissue_dvs_detail' class='fix_width100'>
							    <option value='세금계산서'>세금계산서</option>
                                <option value='지출증빙'>지출증빙</option>
                                <option value='소득공제'>소득공제</option>
                                <option value='미발행'>미발행</option>
                                <option value='카드건별'>카드건별</option>
							</select><button type=\"button\" onclick='UnissuedChangeDvsPeriod(%d, \"%s\", \"%s\",\"%s\",\"%s\",\"%s\");' class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">변경</button></td>";
    $html .= "\n    <td><button type=\"button\" onclick=\"showUnissueCasePopup(%d, '%s','%s', '대기')\" class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">건별보기</button></td>";
    $html .= "\n  </tr>";
    $i = 1 + $param["s_num"];
    $j = 1;
    $prev_channel = "";
    while ($rs && !$rs->EOF) {
        $param["member_seqno"] = $rs->fields["member_seqno"];
        $rs2 = $dao->selectAllPayprice2($conn, $param);
        $object_price = $rs->fields["pay_price"] - $rs->fields["adjust_sales"] - $rs->fields["enuri"];

        if($prev_channel != "" && ($rs->fields["sell_channel"] != $prev_channel))
            $i = 1;
        $prev_channel = $rs->fields["sell_channel"];

        $evid_dvs = "0";
        if ($rs->fields["evid_dvs"] == "지출증빙용") {
            $evid_dvs = "1";
        }

        $rs_html .= sprintf($html,
                $rs->fields["member_pay_history_seqno"],
                $i,
                $rs->fields["sell_channel"],
                $rs->fields["member_name"],
                $rs->fields["corp_name"],
                $rs->fields["crn"],
                $rs->fields["tel_num"],
                $rs->fields["cell_num"],
            number_format($rs2->fields["card_pay_price"] + $rs2->fields["pay_price"] - $rs2->fields["adjust_sales"]),
            number_format($rs2->fields["enuri"]),
            number_format($rs2->fields["card_pay_price"] + $rs2->fields["pay_price"] - $rs2->fields["adjust_sales"] - $rs2->fields["enuri"]),
            number_format($object_price),
                $j,
                $rs->fields["member_seqno"],
                $rs->fields["public_dvs"],
                $rs->fields["dvs_detail"],
                $param["year"],
                $param["mon"],
                $j,
            $rs->fields["member_seqno"],
                $param["year"],
                $param["mon"],

                $rs->fields["tel_num"],
                $rs->fields["member_pay_history_seqno"],
                $rs->fields["member_pay_history_seqno"],
            $rs->fields["member_pay_history_seqno"],
            $rs->fields["member_pay_history_seqno"],
                $rs->fields["member_pay_history_seqno"]
                );

        $i++;
        $j++;
        $rs->moveNext();
    }

    return $rs_html;
}

/**
 * @brief 매출 세금계산서(발행완료)리스트 HTML
 */
function makePublicCompleteListHtml($conn, $dao, $rs, $param) {
  
    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr>";
    $html .= "\n    <td><input type='checkbox' class='check_box' name='standby_chk' value='%d'></td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\"><a href=\"#\"";
    $html .= " onclick=\"publicLayer('basic',";
    $html .= " %d,'%s','%s','%s','%s','%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '')\">";
    $html .= "%s</a></td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td style=\"text-align: right;\">%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td><button type=\"button\" onclick=\"saveStateSeq(%d, '대기')\" class=\"bred btn_pu fix_height20 fix_width60\">발행취소</button><button type=\"button\" onclick=\"showTaxReadyCasePopup(%d, '%s','%s', '완료')\" class=\"btn_Turquoise01 btn_pu fix_height20 fix_width60\">건별보기</button></td>";
    $html .= "\n  </tr>";
    $i = 1 + $param["s_num"];
    $prev_channel = "";

    while ($rs && !$rs->EOF) {
        $param["member_seqno"] = $rs->fields["member_seqno"];
        $rs2 = $dao->selectAllPayprice2($conn, $param);
        $object_price = $rs->fields["pay_price"] - $rs->fields["adjust_sales"] - $rs->fields["enuri"];

        if($prev_channel != "" && ($rs->fields["sell_channel"] != $prev_channel))
            $i = 1;
        $prev_channel = $rs->fields["sell_channel"];

        if($rs->fields["member_name"] == null) {
            $rs->moveNext();
            continue;
        }
        $object_price = $rs->fields["pay_price"] - $rs->fields["adjust_price"];
        if($object_price < 0) $object_price = 0;
        $rs_html .= sprintf($html,
            $rs->fields["public_admin_seqno"],
            $rs->fields["member_name"],
            $rs->fields["crn"],
            $rs->fields["public_admin_seqno"],
            $rs->fields["public_date"],
            $rs->fields["cpn_admin_seqno"],
            $rs->fields["pay_year"] . "년 " .
            $rs->fields["pay_mon"] . "월",
            number_format($rs->fields["pay_price"]),
            number_format($rs->fields["pay_price"]),
            number_format($rs->fields["card_pay_price"]),
            number_format($rs->fields["adjust_price"]),
            $rs->fields["oa"],
            $rs->fields["before_oa"],
            number_format($object_price),
            $rs->fields["corp_name"],
            $rs->fields["repre_name"],
            $rs->fields["crn"],
            $rs->fields["bc"],
            $rs->fields["tob"],
            $rs->fields["addr"],
            $rs->fields["zipcode"],
            $rs->fields["req_mon"],
            $param["day"],
            number_format($rs->fields["unitprice"]),
            number_format($rs->fields["supply_price"]),
            number_format($rs->fields["vat"]),
            $rs->fields["corp_name"],
            $rs->fields["tel_num"],
            $rs->fields["cell_num"],
            number_format($rs2->fields["card_pay_price"] + $rs2->fields["pay_price"] - $rs2->fields["adjust_sales"]),
            number_format($rs2->fields["enuri"]),
            number_format($rs2->fields["card_pay_price"] + $rs2->fields["pay_price"] - $rs2->fields["adjust_sales"] - $rs2->fields["enuri"]),
            number_format($object_price),
            "완료",
            $rs->fields["member_seqno"],
            $rs->fields["member_seqno"],
            $param["year"],
            $param["mon"]
        );

        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}

/**
 * @brief 매출 예외리스트 HTML
 */
function makePublicExceptListHtml($conn, $dao, $rs, $param) {
  
    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr>";
    $html .= "\n    <td><input type='checkbox' class='check_box' name='standby_chk' value='%d'></td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\">%s</td>";
    $html .= "\n    <td style=\"text-align: left;\"><a href=\"#\"";
    $html .= " onclick=\"publicLayer('edit',";
    $html .= " %d,'%s','%s','%s','%s','%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '%s', '%s',";
    $html .= " '%s', '%s', '%s', '%s', '%s', '%s',";
    $html .= " '%s', '%s','%s', '%s', '%s')\">";
    $html .= "%s</a></td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td 'style=\"text-align: right;\"'>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td><button type=\"button\" onclick=\"saveStateSeq(%d, '대기')\" class=\"bred btn_pu btn fix_height20 fix_width40\">삭제</button></td>"; $html .= "\n  </tr>";
    $i = 1 + $param["s_num"];

    while ($rs && !$rs->EOF) {

        $rs_html .= sprintf($html,
                $rs->fields["public_admin_seqno"],
                $rs->fields["req_date"],
                $rs->fields["crn"],
                $rs->fields["public_admin_seqno"],
                $rs->fields["public_date"],
                $rs->fields["cpn_admin_seqno"],
                $rs->fields["req_year"] . "년 " .
                $rs->fields["req_mon"] . "월",
                $rs->fields["pay_price"],
                $rs->fields["card_price"],
                $rs->fields["money_price"],
                $rs->fields["etc_price"],
                $rs->fields["oa"],
                $rs->fields["before_oa"],
                $rs->fields["object_price"],
                $rs->fields["corp_name"],
                $rs->fields["repre_name"],
                $rs->fields["crn"],
                $rs->fields["bc"],
                $rs->fields["tob"],
                $rs->fields["addr"],
                $rs->fields["zipcode"],
                $rs->fields["req_mon"],
                $param["day"],
                $rs->fields["unitprice"],
                $rs->fields["supply_price"],
                $rs->fields["vat"],
                $rs->fields["public_dvs"],
                $rs->fields["corp_name"],
                $rs->fields["tel_num"],
                $rs->fields["req_mon"],
                number_format($rs->fields["pay_price"]),
                number_format($rs->fields["pay_price"]),
                0,
                0,
                number_format($rs->fields["pay_price"]),
                number_format($rs->fields["object_price"]),
                $rs->fields["tab_public"],
                $rs->fields["public_admin_seqno"]
                );

        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}


?>
