<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/common_lib/CommonUtil.inc");
include_once(INC_PATH . "/com/nexmotion/job/nimda/produce/typset_mng/ProcessMngDAO.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once(INC_PATH . '/com/dprinting/MoamoaDAO.inc');



/**
 * @brief 생산공정 구분 정의 인쇄 리스트 HTML
 */
function makeCuttingListHtml($rs, $param) {

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
    $html .= "\n    <td width=\"260px;\">";
    $html .= "\n    <button type=\"button\" class=\"orge\" onclick=\"window.location.href='%s';\">판 PDF</button>";
    $html .= "\n    <button type=\"button\" class=\"orge\" style=\"margin-left: 4px;\" onclick=\"window.location.href='%s';\" >딱판</button>";
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
        $rs_html .= sprintf($html, $class,
            $rs->fields["sheet_typset_seqno"],
            $rs->fields["typset_num"],
            $rs->fields["print_title"],
            $tmpt,
            $rs->fields["paper_name"],
            $rs->fields["print_amt"] . $rs->fields["print_amt_unit"],
            $rs->fields["erp_state_name"],
            $rs->fields["memo"],
            $rs->fields["typset_pdf"],
            $rs->fields["typset_label"],
            $rs->fields["typset_num"],
            $rs->fields["sheet_typset_seqno"],
            $disabled);
        $i--;
        $rs->moveNext();
    }

    return $rs_html;
}

function makeCuttingProcessHtml($rs, $param) {

    if (!$rs) {
        return false;
    }

    $rs_html = "";
    $html  = "\n  <tr class=\"%s\">";
    $html .= "\n    <td style='font-size:14px;'>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    if($param["state"] == "2420") {
        $html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"getStart('%s', '2430');\" style='display: block'>작업시작</button></td>";
    }
    if($param["state"] == "2430") {
        $html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"getStart('%s', '2480');\" style='display: block'>작업완료</button></td>";
    }
    if($param["state"] == "2480") {
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
        if($param["state"] == "2480") {
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
function makeCuttingProcessHtml2($rs, $param) {

    if (!$rs) {
        return false;
    }

    $connectionPool = new ConnectionPool();
    $conn = $connectionPool->getPooledConnection();

    $rs_html = "";
    $html  = "\n  <tr class=\"%s\">";
    $html .= "\n    <td style='font-size:14px;'>%s</td>";
    $html .= "\n    <td style='font-size:14px;'>%s</td>";
    $html .= "\n    <td>%s</td>";

    if($param["state"] == "2420") {
        $html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"getStart('%s', '2430');\" style='display: block'>작업시작</button></td>";
    }
    if($param["state"] == "2430") {
        $html .= "\n    <td width=\"260px;\"><button type=\"button\" class=\"orge\" onclick=\"getStart('%s', '2480');\" style='display: block'>작업완료</button></td>";
    }
    if($param["state"] == "2480") {
        $html .= "\n    <td width=\"260px;\">%s</td>";
    }
    //$html .= "\n    <td width=\"260px;\">";
    $html .= "\n    </td>";
    $html .= "\n  </tr>";

    $i = $param["cnt"] - $param["s_num"];
     if( $param["after"] == 'y'){
        $test = 1;
    }else{
        $test = 0;
    }

    $param2 = array();
    $check = array();
    $util = new CommonUtil();
    $dao2 = new MoamoaDAO();
    $dao = new ProcessMngDAO();
    while ($rs && !$rs->EOF) {

        

        if($test == 1){
            $param2["order_detail_num"]  = "";
        }else{
            if(!in_array($rs->fields["order_num"], $check)){
               
                array_push($check, $rs->fields["order_num"]);

                $param2["order_detail_num"]  = $rs->fields["order_num"];
                $param2["order_num"]  = $rs->fields["order_num"];
                $param2["order_common_seqno"]  = $rs->fields["order_common_seqno"];
                $param2["state"] = 3220;
             
                $param['empl_id'] = $session["id"];
                $param['state'] = 3220;
                $param['ordernum'] = $rs->fields["order_num"];
                

          
                // $rs2 = $dao->updateOrderState2($conn, $param2);
                $dao2->updateProductStatecode($conn, $param);
                $dao2->insertStateHistory($conn, $param);
               
                if($rs2 != null) {
                    $result =  "1";
                } else {
                    $result =  "0";
                }
            }else{
                $param2["order_detail_num"]  = "";
                $param2["state"] = "";
                $rs2 = "";
                $result =  "0";

            }
            
        }
       
       

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
        if($param["state"] == "2480") {
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
                $rs->fields["order_num"],
                //$rs->fields["title"],
                $rs->fields["member_name"],
              //  $rs->fields["amt"],
             //   $param2["order_detail_num"],
             //   $disabled
                );
        }
        $i--;
        $rs->moveNext();
    }

    return $rs_html;
}


?>
