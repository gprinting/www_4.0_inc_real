<?php
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/common_lib/CommonUtil.inc");

/**
 * @brief 생산공정 구분 정의 출력 리스트 HTML
 */
function makeStorageListHtml($rs, $param,$dao, $conn) {

    if (!$rs) {
        return false;
    }

    $util = new CommonUtil();
    $rs_html = "";
    $html  = "\n  <tr>";
    $html .= "\n    <td class='font_size1'>%s</td>"; // NO
    $html .= "\n    <td class='font_size1'>%s</td>"; // 주문일
    $html .= "\n    <td class='font_size1'>%s</td>"; // 판번호
    $html .= "\n    <td class='font_size1'>%s</td>"; // 주문번호
    $html .= "\n    <td class='font_size1'>%s</td>"; // 회원명
    $html .= "\n    <td class='font_size1'>%s</td>"; // 인쇄물제목
    $html .= "\n    <td class='font_size1'>%s</td>"; // 상품정보
    $html .= "\n    <td class='font_size1'>%s</td>"; // 매수
    $html .= "\n    <td class='font_size1'>%s</td>"; // 건수
    $html .= "\n    <td class='font_size1'>%s</td>"; // 배송
    $html .= "\n    <td class='font_size1'>%s</td>"; // 배송
    $html .= "\n    <td class='font_size1'>%s</td>"; // 배송
    $html .= "\n    <td class='font_size1'><button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width60\" onclick=\"showDlvrDetail('%s');\">인쇄</button></td>"; // 인쇄
    //$html .= "\n    <td class='font_size1'>%s</td>"; // 상태
    //$html .= "\n    <td class='font_size1'>%s</td>"; // 상태
    //$html .= "\n    <td class='font_size1'>%s</td>"; // 상태
    //$html .= "\n    <td><button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width60\" onclick=\"changeState('%s');\">강제입고</button>"; // 관리
    $html .= "\n  </tr>";

    $i = 1;

    while ($rs && !$rs->EOF) {
        $typset_num = $rs->fields["typset_num"];
        if($typset_num == null)
            $typset_num = $rs->fields["receipt_finish_date"];
        $rs->fields['order_regi_date'] = substr($rs->fields['order_regi_date'],0,10);
        $rs->fields['receipt_regi_date'] = substr($rs->fields['receipt_regi_date'],0,10);
        $state = $util->statusCode2status($rs->fields["order_state"]);


        $dlvr_way = "";
        if ($rs->fields["dlvr_way"] == "01") {
            $dlvr_way = "택배";
        } else if ($rs->fields["dlvr_way"] == "02"){
            $dlvr_way = "직배(" . $rs->fields["invo_cpn"] . ")";
        } else if ($rs->fields["dlvr_way"] == "03"){
            $dlvr_way = "화물";
        } else if ($rs->fields["dlvr_way"] == "04"){
            $dlvr_way = "퀵(오토바이)";
        } else if ($rs->fields["dlvr_way"] == "05"){
            $dlvr_way = "퀵(지하철)";
        } else if ($rs->fields["dlvr_way"] == "06"){
            $dlvr_way = "방문(인현동)";
        } else if ($rs->fields["dlvr_way"] == "07"){
            $dlvr_way = "방문(성수동)";
        }

        $size = explode(' / ', $rs->fields["order_detail"])[2];
        if($size == "비규격") {
            $size = $rs->fields["cut_size_wid"] . "*" . $rs->fields["cut_size_vert"];
        }

        $temp = array();
        $temp["order_common_seqno"] = $rs->fields["order_common_seqno"];
        $order_detail = $dao->selectOrderAfterInfo($conn, $temp);

        $state = "";
        if($rs->fields["order_state"] == "3220") {
            $state = "입고완료";
        } else if($rs->fields["order_state"] == "3320") {
            $state = "출고완료";
        }

        $rs_html .= sprintf($html,
            $i++,
            //$rs->fields["order_regi_date"],
            $typset_num,
            $rs->fields["member_name"],
            $rs->fields["title"],
            explode(' / ', $rs->fields["order_detail"])[1],
            $rs->fields["amt"] / 10 * 10,
            $rs->fields["count"],
            //$dlvr_way,
            $size,
            $order_detail,
            $rs->fields["dlvr_req"],
            //$rs->fields["state_modi_date"],
            $state, $dlvr_way,
            $rs->fields["order_common_seqno"]); // 업체번호);

        $rs->moveNext();
    }

    if ($rs_html == "") {
        return "<tr><td colspan=\"10\">검색된 내용이 없습니다.</td></tr>";
    }

    return $rs_html;
}

/**
 * @brief 생산공정 구분 정의 출력 리스트 HTML
 */
function makeDeliveryListHtml($conn, $dao, $rs, $param) {

    if (!$rs) {
        return false;
    }

    $util = new CommonUtil();
    $rs_html = "";
    $html  = "\n  <tr>";
    $html .= "\n    <td>%s</td>"; // NO
    $html .= "\n    <td>%s</td>"; // 주문일
    $html .= "\n    <td onclick='click_ordernum(\"%s\")'>%s</td>"; // 주문번호
    $html .= "\n    <td>%s</td>"; // 회원명
    $html .= "\n    <td>%s</td>"; // 인쇄물제목
    $html .= "\n    <td>%s</td>"; // 상품정보
    $html .= "\n    <td>%s</td>"; // 후가공
    $html .= "\n    <td>%s</td>"; // 송장번호
    $html .= "\n    <td>%s</td>"; // 매수
    $html .= "\n    <td>%s</td>"; // 건수
    $html .= "\n    <td>%s</td>"; // 상태
    $html .= "\n    <td>%s</td>"; // 배송비
    $html .= "\n    <td>%s</td>"; // 주소
    $html .= "\n    <td>"; // 관리
    $html .= "\n    <button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width60\" style='%s' onclick=\"showDlvrWith('%s', '%s');\">묶음보기</button>"; // 관리
    $html .= "\n    <button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width60\" style='%s' onclick=\"openBunDlvr('%s');\">묶음출력</button>"; // 관리
    $html .= "\n  </tr>";

    $i = 1;

    while ($rs && !$rs->EOF) {
        $rs->fields['order_regi_date'] = substr($rs->fields['order_regi_date'], 0, 10);
        $rs->fields['receipt_regi_date'] = substr($rs->fields['receipt_regi_date'], 0, 10);
        $state = $util->statusCode2status($rs->fields["order_state"]);

        $dlvr_way = "";
        if ($rs->fields["dlvr_way"] == "01") {
            $dlvr_way = "택배";
        } else if ($rs->fields["dlvr_way"] == "02") {
            $dlvr_way = "직배";
        } else if ($rs->fields["dlvr_way"] == "03") {
            $dlvr_way = "화물";
        } else if ($rs->fields["dlvr_way"] == "04") {
            $dlvr_way = "퀵(오토바이)";
        } else if ($rs->fields["dlvr_way"] == "05") {
            $dlvr_way = "퀵(지하철)";
        } else if ($rs->fields["dlvr_way"] == "06") {
            $dlvr_way = "방문(인현동)";
        } else if ($rs->fields["dlvr_way"] == "07") {
            $dlvr_way = "방문(성수동)";
        }

        if ($rs->fields["dlvr_sum_way"] == "01") {
            $dlvr_sum_way = "선불";
        } else if ($rs->fields["dlvr_sum_way"] == "02") {
            $dlvr_sum_way = "후불";
        }

        $str_memo = $dao->selectPackageCount($conn, $rs->fields["bun_dlvr_order_num"], $rs->fields["order_common_seqno"], $rs->fields["dlvr_dvs"], $rs->fields["member_seqno"]);
        $str_hidden = "display : none";
        $str_hidden2 = "display : none";
        if ($rs->fields["cnt"] > 1) {
            $str_hidden2 = "";
        }
        if($str_memo > 1) {
            $str_hidden = "";
        }

        $temp1 = array();
        $temp1["order_common_seqno"] = $rs->fields["order_common_seqno"];
        $after_detail = $dao->selectOrderAfterInfoForParsel($conn, $temp1);

        $rs_html .= sprintf($html,
            $i,
            $rs->fields["order_regi_date"],
            $rs->fields["order_num"],
            $rs->fields["order_num"],
            $rs->fields["member_name"],
            $rs->fields["title"],
            $rs->fields["order_detail"],
            $after_detail,
            $rs->fields["invo_num"],
            number_format($rs->fields["amt"]) . $rs->fields["amt_unit_dvs"],
            $rs->fields["count"],
            $dlvr_sum_way,
            $rs->fields["dlvr_price"],
            "(" . $rs->fields["zipcode"] . ")" . $rs->fields["addr"] . $rs->fields["addr_detail"],
            $str_hidden,
            $rs->fields["bun_dlvr_order_num"],
            $rs->fields["member_seqno"],
            $str_hidden2,
            $rs->fields["order_common_seqno"]); // 업체번호);
        $i++;
        $rs->moveNext();
    }

    if ($rs_html == "") {
        return "<tr><td colspan=\"12\">검색된 내용이 없습니다.</td></tr>";
    }

    return $rs_html;
}

?>