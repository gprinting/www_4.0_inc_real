<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/common_lib/CommonUtil.inc");

/**
 * @brief 생산공정 구분 정의 조판 후공정 리스트 HTML
 */
function makeReleaseListHtml($rs, $param) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class=\"%s\">";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    if($param["state"] == "2720") {
        $html .= "\n    <td width=\"150px;\"><button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width60\" onclick=\"multiFinish('%s', '2780');\">완료</button></td>";
    } else {
        $html .= "\n    <td width=\"150px;\">%s</td>";
    }
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];

    $util = new CommonUtil();

    while ($rs && !$rs->EOF) {

        $state_arr = $_SESSION["state_arr"];
        if ($rs->fields["state"] != $state_arr["주문후공정준비"]) {

            if ($i % 2 == 0) {
                $class = "cellbg";
            } else if ($i % 2 == 1) {
                $class = "";
            }

            $state = $util->statusCode2status($rs->fields["state"]);
            if ($state == "") {
                $state = "후공정대기";
            }

            $amt = "";
            if ($rs->fields["amt"] < 1) {
                $amt = number_format($rs->fields["amt"], 1);
            } else {
                $amt = number_format($rs->fields["amt"]);
            }

            if($param["state"] == "2720") {
                $rs_html .= sprintf($html, $class,
                    $rs->fields["order_num"],
                    $rs->fields["after_name"],
                    $rs->fields["depth1"],
                    $rs->fields["depth2"],
                    $rs->fields["depth3"],
                    $amt . $rs->fields["amt_unit_dvs"],
                    $rs->fields["work_memo"],
                    $state,
                    $rs->fields["order_after_history_seqno"],
                    $rs->fields["order_after_history_seqno"]);
            } else {
                $rs_html .= sprintf($html, $class,
                    $rs->fields["order_num"],
                    $rs->fields["after_name"],
                    $rs->fields["depth1"],
                    $rs->fields["depth2"],
                    $rs->fields["depth3"],
                    $amt . $rs->fields["amt_unit_dvs"],
                    $rs->fields["work_memo"],
                    $state,
                    $rs->fields["state_modi_date"],
                    $rs->fields["state_modi_date"]);
            }
            $i--;
        }
        $rs->moveNext();
    }

    return $rs_html;
}

?>