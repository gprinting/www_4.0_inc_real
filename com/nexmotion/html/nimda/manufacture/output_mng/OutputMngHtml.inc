<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/common_lib/CommonUtil.inc");

/**
 * @brief 생산공정 구분 정의 출력 리스트 HTML
 */
function makeOutputListHtml($rs, $param, $dao, $conn) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class=\"%s\">";
    $html .= "\n    <td %s><input type=\"checkbox\" name=\"chk\" value=\"%s\"></td>";
    $html .= "\n    <td id=\"%s\">%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"openDetailView('%s');\">보기</button>  <button type=\"button\" id=\"output_seq_%s\" class=\"%s\" onclick=\"openPaperOpView('%s');\">종이발주</button> <button type=\"button\" class=\"%s\" onclick=\"openPrint('%s');\" >작업지시서</button><!--button type=\"button\" class=\"orge\" style=\"margin-left: 4px;\" onclick=\"openLabelView('%s');\" >딱판</button--></td>";
    //$html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"openImgView('%s');\">판 PDF</button><button type=\"button\" class=\"orge\" style=\"margin-left: 4px;\" onclick=\"openLabelView('%s');\" >딱판</button></td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];

    $util = new CommonUtil();

    while ($rs && !$rs->EOF) {
        $bg_color = "";
        $print_style = "orge";
        if($rs->fields["printed"]) {
            $print_style = "gray";
            $bg_color = "style='background:lightgreen;'";
        }

        $state_arr = $_SESSION["state_arr"];
        if ($rs->fields["state"] != $state_arr["출력준비"]) {

            if ($i % 2 == 0) {
                $class = "cellbg";
            } else if ($i % 2 == 1) {
                $class = "";
            }

            $state = $util->statusCode2status($rs->fields["state"]);

            if ($state == "인쇄대기" || $state == "후공정대기") {
                $state = "출력완료";
            }

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
            $param1 = array();
            $param1["table"] = "paper_op";
            $param1["col"] = "paper_op_seqno";
            $param1["where"]["typset_num"] = $rs->fields["typset_num"];
            $paper_op_seqno = $dao->selectData($conn, $param1)->fields["paper_op_seqno"];

            $paper_op_style = "gray";
            if($paper_op_seqno == "")
                $paper_op_style = "orge";
            $size = $rs->fields["print_title"];

            $preset_name_arr = explode("_", $rs->fields["preset_name"]);
            $tmpt_info = $preset_name_arr[2];
            $tmpt = ($rs->fields["beforeside_tmpt"] + $rs->fields["aftside_tmpt"]) . "도(" .  ($rs->fields["prdt_page"] == 2 ? "양면)" : "단면)");
            $rs_html .= sprintf($html, $class,
                $bg_color,
                    $rs->fields["sheet_typset_seqno"],
                    "seqno_" . $rs->fields["sheet_typset_seqno"],
                    $rs->fields["typset_num"],
                    $rs->fields["print_title"],
                    $tmpt,
                    $rs->fields["paper_name"],
                    $rs->fields["print_amt"] . $rs->fields["print_amt_unit"],
                    $rs->fields["memo"],
                    $rs->fields["erp_state_name"],
                    $rs->fields["sheet_typset_seqno"],
                    $rs->fields["sheet_typset_seqno"],
                    $paper_op_style,
                    $rs->fields["sheet_typset_seqno"],
                    $print_style,
                    $rs->fields["typset_num"],
                    $rs->fields["typset_num"],
                    $disabled);
            $i--;
        }
        $rs->moveNext();
    }

    return $rs_html;
}

function makeImpositionListHtml($rs, $param) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class=\"%s\">";
    $html .= "\n    <td %s><input type=\"checkbox\" name=\"chk\" value=\"%s\"></td>";
    $html .= "\n    <td onclick='click_impose_num(\"%s\")' style='cursor: pointer;'>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    //$html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"openDetailView('%s');\">보기</button> <button type=\"button\" class=\"orge\" onclick=\"openImgView('%s');\">판 PDF</button> <button type=\"button\" class=\"orge\" onclick=\"openPrint('%s');\" >인쇄</button><button type=\"button\" class=\"orge\" style=\"margin-left: 4px;\" onclick=\"openLabelView('%s');\" >딱판</button></td>";
    $html .= "\n    <td width=\"260px;\">";
    $html .= "\n    <button type=\"button\" class=\"orge\" onclick=\"downloadURI('%s','판PDF');\">판 PDF</button>";
    $html .= "\n    <button type=\"button\" class=\"orge\" style=\"margin-left: 4px;\" onclick=\"downloadURI('%s','딱판PDF');\" >딱판(합판)</button>";
    $html .= "\n    <button type=\"button\" class=\"orge\" style=\"margin-left: 4px;\" onclick=\"make_label('%s');\" >딱판(낱건)</button>";
    $html .= "\n    </td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];

    $util = new CommonUtil();

    while ($rs && !$rs->EOF) {
        $bg_color = "";
        if($rs->fields["printed"]) {
            $bg_color = "style='background:lightgreen;'";
        }
        $state_arr = $_SESSION["state_arr"];
        if ($rs->fields["state"] != $state_arr["출력준비"]) {

            if ($i % 2 == 0) {
                $class = "cellbg";
            } else if ($i % 2 == 1) {
                $class = "";
            }

            $state = $util->statusCode2status($rs->fields["state"]);

            if ($state == "인쇄대기" || $state == "후공정대기") {
                $state = "출력완료";
            }

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
            $rs_html .= sprintf($html, $class,
                $bg_color,
                $rs->fields["sheet_typset_seqno"],
                $rs->fields["typset_num"],
                $rs->fields["typset_num"],
                $rs->fields["print_title"],
                $tmpt,
                $rs->fields["paper_name"],
                $rs->fields["print_amt"] . $rs->fields["print_amt_unit"],
                $rs->fields["erp_state_name"],
                $rs->fields["memo"],
                $rs->fields["typset_pdf"],
                $rs->fields["typset_label"],
                $rs->fields["sheet_typset_seqno"],
                $disabled);
            $i--;
        }
        $rs->moveNext();
    }

    return $rs_html;
}

function makeImpositionListHtml2($rs, $param) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    //$html  = "\n  <select class=\"%s\">";
    //$html .= "\n    <option value=\"%s\">%s</option>";
    //$html .= "\n  </select>";
    $html  = "\n  <tr>";
    $html .= "\n    <td><input type=\"checkbox\" name=\"chk\" value=\"%s\"></td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];

    $util = new CommonUtil();

    while ($rs && !$rs->EOF) {
        $bg_color = "";
        if($rs->fields["printed"]) {
            $bg_color = "style='background:lightgreen;'";
        }
        $state_arr = $_SESSION["state_arr"];
        if ($rs->fields["state"] != $state_arr["출력준비"]) {

            if ($i % 2 == 0) {
                $class = "cellbg";
            } else if ($i % 2 == 1) {
                $class = "";
            }

            $state = $util->statusCode2status($rs->fields["state"]);

            if ($state == "인쇄대기" || $state == "후공정대기") {
                $state = "출력완료";
            }

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
            $rs_html .= sprintf($html,$rs->fields["typset_num"],
                $rs->fields["typset_num"]);
            $i--;
        }
        $rs->moveNext();
    }

    return $rs_html;
}

function makeOutputProcessHtml($rs, $param) {

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
    if($param["state"] == "2220") {
        $html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"getStart('%s', '2320');\" >작업시작</button></td>";
    }
    if($param["state"] == "2230") {
        $html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"getStart('%s', '2320');\" style='display: none'>작업완료</button></td>";
    }
    if($param["state"] == "2280") {
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

        if ($state == "조판후공정대기") {
            $state = "재단완료";
        }

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
        if($param["state"] == "2280") {
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


function getEnvelopeValue($key)
{
    $size = "";
    switch ($key) {
        case "국전-대봉2개-소봉4개|모조지":
        case "국전-대봉2개-소봉4개|100 모조":
        case "국전-대봉2개-소봉4개|120 모조":
        case "국전-대봉2개-소봉4개|150 모조":
        case "8색기-국전-대봉2개-소봉4개|모조지":
        case "8색기-국전-대봉2개-소봉4개|100 모조":
        case "8색기-국전-대봉2개-소봉4개|120 모조":
        case "8색기-국전-대봉2개-소봉4개|150 모조":
            $size = "1020 x 670";
            break;
        case "국전-대봉2개-소봉4개|레자크 #91(체크)":
        case "국전-대봉2개-소봉4개|#91 백색 레쟈크":
        case "8색기-국전-대봉2개-소봉4개|레자크 #91(체크)":
        case "8색기-국전-대봉2개-소봉4개|#91 백색 레쟈크":
            $size = "1020 x 670";
            break;
        case "국전-대봉2개-소봉4개|레자크 #92(줄)":
        case "국전-대봉2개-소봉4개|#92 백색 레쟈크":
        case "8색기-국전-대봉2개-소봉4개|레자크 #92(줄)":
        case "8색기-국전-대봉2개-소봉4개|#92 백색 레쟈크":
            $size = "1020 x 670";
            break;

        case "2절-대봉2개|모조지":
        case "2절-대봉2개|100 모조":
        case "2절-대봉2개|120 모조":
        case "2절-대봉2개|150 모조":
        case "우성-2절-대봉2개|모조지":
        case "우성-2절-대봉2개|100 모조":
        case "우성-2절-대봉2개|120 모조":
        case "우성-2절-대봉2개|150 모조":
        case "8색기-2절-대봉2개|모조지":
        case "8색기-2절-대봉2개|100 모조":
        case "8색기-2절-대봉2개|120 모조":
        case "8색기-2절-대봉2개|150 모조":
            $size = "545 x 788";break;
        case "2절-대봉2개|레자크 #91(체크)":
        case "2절-대봉2개|#91 백색 레쟈크":
        case "우성-2절-대봉2개|레자크 #91(체크)":
        case "우성-2절-대봉2개|#91 백색 레쟈크":
        case "8색기-2절-대봉2개|레자크 #91(체크)":
        case "8색기-2절-대봉2개|#91 백색 레쟈크":
            $size = "506 x 788";break;
        case "2절-대봉2개|레자크 #92(줄)":
        case "2절-대봉2개|#92 백색 레쟈크":
        case "우성-2절-대봉2개|레자크 #92(줄)":
        case "우성-2절-대봉2개|#92 백색 레쟈크":
        case "8색기-2절-대봉2개|레자크 #92(줄)":
        case "8색기-2절-대봉2개|#92 백색 레쟈크":
            $size = "506 x 788";break;


        case "국전-소봉투(12개)|모조지":
        case "국전-소봉투(12개)|100 모조":
        case "국전-소봉투(12개)|120 모조":
        case "국전-소봉투(12개)|150 모조":
        case "8색기-국전-소봉투(12개)|모조지":
        case "8색기-국전-소봉투(12개)|100 모조":
        case "8색기-국전-소봉투(12개)|120 모조":
        case "8색기-국전-소봉투(12개)|150 모조":
            $size = "1060 x 740";break;
        case "국전-소봉투(12개)|레자크 #91(체크)":
        case "국전-소봉투(12개)|#91 백색 레쟈크":
        case "8색기-국전-소봉투(12개)|레자크 #91(체크)":
        case "8색기-국전-소봉투(12개)|#91 백색 레쟈크":
            $size = "1060 x 740";break;
        case "국전-소봉투(12개)|레자크 #92(줄)":
        case "국전-소봉투(12개)|#92 백색 레쟈크":
        case "8색기-국전-소봉투(12개)|레자크 #92(줄)":
        case "8색기-국전-소봉투(12개)|#92 백색 레쟈크":
            $size = "1060 x 740";break;


        case "2절-대봉1개-소봉2개|모조지":
        case "2절-대봉1개-소봉2개|100 모조":
        case "2절-대봉1개-소봉2개|120 모조":
        case "2절-대봉1개-소봉2개|150 모조":
        case "우성-2절-대봉1개-소봉2개|모조지":
        case "우성-2절-대봉1개-소봉2개|100 모조":
        case "우성-2절-대봉1개-소봉2개|120 모조":
        case "우성-2절-대봉1개-소봉2개|150 모조":
        case "8색기-2절-대봉1개-소봉2개|모조지":
        case "8색기-2절-대봉1개-소봉2개|100 모조":
        case "8색기-2절-대봉1개-소봉2개|120 모조":
        case "8색기-2절-대봉1개-소봉2개|150 모조":
            $size = "545 x 788";break;
        case "2절-대봉1개-소봉2개|레자크 #91(체크)":
        case "2절-대봉1개-소봉2개|#91 백색 레쟈크":
        case "우성-2절-대봉1개-소봉2개|레자크 #91(체크)":
        case "우성-2절-대봉1개-소봉2개|#91 백색 레쟈크":
        case "8색기-2절-대봉1개-소봉2개|레자크 #91(체크)":
        case "8색기-2절-대봉1개-소봉2개|#91 백색 레쟈크":
            $size = "506 x 788";break;
        case "2절-대봉1개-소봉2개|레자크 #92(줄)":
        case "2절-대봉1개-소봉2개|#92 백색 레쟈크":
        case "우성-2절-대봉1개-소봉2개|레자크 #92(줄)":
        case "우성-2절-대봉1개-소봉2개|#92 백색 레쟈크":
        case "8색기-2절-대봉1개-소봉2개|레자크 #92(줄)":
        case "8색기-2절-대봉1개-소봉2개|#92 백색 레쟈크":
            $size = "788 x 506";break;


        case "2절-대봉1개-소봉1개-이삿짐봉투1개|모조지":
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|100 모조":
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|120 모조":
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|150 모조":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|모조지":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|100 모조":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|120 모조":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|150 모조":
            $size = "545 x 788";break;
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|레자크 #91(체크)":
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|#91 백색 레쟈크":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|레자크 #91(체크)":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|#91 백색 레쟈크":
            $size = "788 x 506";break;
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|레자크 #92(줄)":
        case "2절-대봉1개-소봉1개-이삿짐봉투1개|#92 백색 레쟈크":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|레자크 #92(줄)":
        case "8색기-2절-대봉1개-소봉1개-이삿짐봉투1개|#92 백색 레쟈크":
            $size = "788 x 506";break;


        case "2절-소봉6개|모조지":
        case "2절-소봉6개|100 모조":
        case "2절-소봉6개|120 모조":
        case "2절-소봉6개|150 모조":
        case "8색기-2절-소봉6개|모조지":
        case "8색기-2절-소봉6개|100 모조":
        case "8색기-2절-소봉6개|120 모조":
        case "8색기-2절-소봉6개|150 모조":
            $size = "788 x 545";break;
        case "2절-소봉6개|레자크 #91(체크)":
        case "2절-소봉6개|#91 백색 레쟈크":
        case "8색기-2절-소봉6개|레자크 #91(체크)":
        case "8색기-2절-소봉6개|#91 백색 레쟈크":
            $size = "788 x 545";break;
        case "2절-소봉6개|레자크 #92(줄)":
        case "2절-소봉6개|#92 백색 레쟈크":
        case "8색기-2절-소봉6개|레자크 #92(줄)":
        case "8색기-2절-소봉6개|#92 백색 레쟈크":
            $size = "788 x 545";break;
        default:
            $size = "";
    }

    return $size;
}
?>
