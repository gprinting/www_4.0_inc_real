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
    $html .= "\n    <td onclick=\"showDetail('%s')\">%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    //$html .= "\n    <td>%s</td>";
    //$html .= "\n    <td>%s</td>";
    if($param["state"] == "3120") {
        //$html .= "\n    <td width=\"150px;\"><button type=\"button\" class=\"bgreen btn_pu btn fix_height20 fix_width60\" onclick=\"multiFinish('%s', '2780');\">완료</button></td>";
    } else {
        //$html .= "\n    <td width=\"150px;\">%s</td>";
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

            $rs_html .= sprintf($html, $class,
                explode(' ',
                    $rs->fields["state_modi_date"])[0],
                $rs->fields["order_num"],
                $rs->fields["member_name"],
                explode(' ',
                    $rs->fields["order_regi_date"])[0],
                $rs->fields["order_detail"],
                $rs->fields["addr"] . " ". $rs->fields["addr_detail"],
                $amt . $rs->fields["amt_unit_dvs"] . " x " . $rs->fields["count"],
            );
            $i--;
        }
        $rs->moveNext();
    }

    return $rs_html;
}

?>