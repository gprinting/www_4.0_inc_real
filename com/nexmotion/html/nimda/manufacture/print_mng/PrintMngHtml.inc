<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/common_lib/CommonUtil.inc");

/**
 * @brief 생산공정 구분 정의 인쇄 리스트 HTML
 */
function makePrintListHtml($rs, $param) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class=\"%s\">";
    $html .= "\n    <td><input type=\"checkbox\" name=\"chk\" value=\"%s\"></td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    //$html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"openDetailView('%s');\">보기</button> <button type=\"button\" class=\"orge\" onclick=\"openImgView('%s');\">판 PDF</button> <button type=\"button\" class=\"orge\" onclick=\"openPrint('%s');\" >인쇄</button><button type=\"button\" class=\"orge\" style=\"margin-left: 4px;\" onclick=\"openLabelView('%s');\" >딱판</button></td>";
    $html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"openImgView('%s');\">판 PDF</button><button type=\"button\" class=\"orge\" style=\"margin-left: 4px;\" onclick=\"openLabelView('%s');\" >딱판</button></td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];

    $util = new CommonUtil();

    while ($rs && !$rs->EOF) {

        $state_arr = $_SESSION["state_arr"];
        if ($rs->fields["state"] != $state_arr["인쇄준비"]) {

            if ($i % 2 == 0) {
                $class = "cellbg";
            } else if ($i % 2 == 1) {
                $class = "";
            }

            $state = $util->statusCode2status($rs->fields["state"]);

            if ($state == "조판후공정대기") {
                $state = "인쇄완료";
            }

            $preset_name_arr = explode("_", $rs->fields["preset_name"]);
 
            $amt = "";
            if ($rs->fields["amt"] < 1) {
                $amt = number_format($rs->fields["amt"], 1);
            } else {
                $amt = number_format($rs->fields["amt"]);
            }

            $preset_name_arr = explode("_", $rs->fields["preset_name"]);
            $tmpt_info = $preset_name_arr[2];
            $tmpt = ($rs->fields["beforeside_tmpt"] + $rs->fields["aftside_tmpt"]) . "도(" .  ($rs->fields["prdt_page"] == 2 ? "양면)" : "단면)");
            $rs_html .= sprintf($html, $class,
                $rs->fields["sheet_typset_seqno"],
                $rs->fields["typset_num"],
                $rs->fields["print_title"],
                $tmpt,
                $rs->fields["paper_name"],
                $rs->fields["print_amt"] . $rs->fields["print_amt_unit"],
                $rs->fields["memo"],
                $rs->fields["erp_state_name"],
                $rs->fields["sheet_typset_seqno"],
                $rs->fields["sheet_typset_seqno"],
                $rs->fields["typset_num"],
                $rs->fields["sheet_typset_seqno"],
                $disabled);
            $i--;
        }
        $rs->moveNext();
    }

    return $rs_html;
}

function makePrintProcessHtml($rs, $param) {

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
    if($param["state"] == "2320") {
        $html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"getStart('%s', '2330');\" style='display: block'>작업시작</button></td>";
    }
    if($param["state"] == "2330") {
        $html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"getStart('%s', '2420');\" style='display: block'>작업완료</button></td>";
    }
    if($param["state"] == "2380") {
        $html .= "\n    <td width=\"260px;\">%s</td>";
    }
    //$html .= "\n    <td width=\"260px;\">";
    $html .= "\n    </td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];

    $util = new CommonUtil();
    while ($rs && !$rs->EOF) {
        $state_arr = $_SESSION["state_arr"];
        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }

        $state = $util->statusCode2status($rs->fields["state"]);

        $preset_name_arr = explode("_", $rs->fields["preset_name"]);

        $amt = "";
        if ($rs->fields["amt"] < 1) {
            $amt = number_format($rs->fields["amt"], 1);
        } else {
            $amt = number_format($rs->fields["amt"]);
        }

        $disabled = "";
        if ($rs->fields["flattyp_dvs"] == "Y" && strrpos((string)$rs->fields["typset_num"], 'J') === false) {
            $disabled = "disabled=\"disabled\"";
        } else {
            $disabled = "";
        }

        $preset_name_arr = explode("_", $rs->fields["preset_name"]);
        $tmpt_info = $preset_name_arr[2];
        $tmpt = ($rs->fields["beforeside_tmpt"] + $rs->fields["aftside_tmpt"]) . "도(" .  ($rs->fields["prdt_page"] == 2 ? "양면)" : "단면)");
        if($param["state"] == "2380") {
            $rs_html .= sprintf($html, $class,
                $rs->fields["typset_num"],
                $rs->fields["paper_name"],
                $tmpt,
                $rs->fields["print_amt"] . $rs->fields["print_amt_unit"],
                $rs->fields["memo"],
                $rs->fields["state_modi_date"],
                $disabled);
        } else {
            $rs_html .= sprintf($html, $class,
                $rs->fields["typset_num"],
                $rs->fields["paper_name"],
                $tmpt,
                $rs->fields["print_amt"] . $rs->fields["print_amt_unit"],
                $rs->fields["memo"],
                $rs->fields["sheet_typset_seqno"],
                $disabled);
        }
        $i--;
        $rs->moveNext();
    }

    return $rs_html;
}
?>
