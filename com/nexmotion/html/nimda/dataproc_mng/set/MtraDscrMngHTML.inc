<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/com/nexmotion/doc/nimda/dataproc_mng/set/MtraDscrMngDOC.inc");

/* 
 * 종이 설명 list  생성 
 * $result : $result->fields["name"] = "종이명" 
 * $result : $result->fields["dvs"] = "구분" 
 * $result : $result->fields["paper_sense"] = "느낌" 
 * $result : $result->fields["paper_dscr_seqno"] = "종이 설명 일련번호" 
 * 
 * return : list
 */
function makePaperDscrList($conn, $result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    while ($result && !$result->EOF) {

        $top         = $result->fields["top"];
        $name        = $result->fields["name"];
        $dvs         = $result->fields["dvs"];
        $color       = $result->fields["color"];
        $basisweight = $result->fields["basisweight"];
        $dscr        = $result->fields["dscr"];
        $purp        = $result->fields["purp"];
        $sense       = $result->fields["paper_sense"];
        $able_after  = $result->fields["able_after"];
        $warn        = $result->fields["warn"];
        $paper_dscr_seqno = $result->fields["paper_dscr_seqno"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>"; 
        $list .= "\n    <td><button type=\"button\" onclick=\"popPaperDscr(%d)\" class=\"orge btn_pu btn fix_height20 fix_width40\">수정</button></td>";

        $ret .= sprintf($list, $i
                             , $top
                             , $name
                             , $dvs
                             , $color
                             , $basisweight
                             , $dscr
                             , $purp
                             , $sense
                             , $able_after
                             , $warn
                             , $paper_dscr_seqno); 

        $result->moveNext();
        $i++;
    }

    return $ret;
}

/* 
 * 후공정 설명 list  생성 
 * $result : $result->fields["name"] = "후공정명" 
 * $result : $result->fields["dscr"] = "설명" 
 * $result : $result->fields["after_dscr_seqno"] = "후공정 설명 일련번호" 
 * 
 * return : list
 */
function makeAfterDscrList($conn, $result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    while ($result && !$result->EOF) {

        $name = $result->fields["name"];
        $dscr = $result->fields["dscr"];
        $after_dscr_seqno = $result->fields["after_dscr_seqno"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n    <td>%d</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>"; 
        $list .= "\n    <td><button type=\"button\" onclick=\"popAfterDscr(%d)\" class=\"orge btn_pu btn fix_height20 fix_width40\">수정</button></td>";

        $ret .= sprintf($list, $i, $name, $dscr, $after_dscr_seqno); 

        $result->moveNext();
        $i++;
    }

    return $ret;
}

/* 
 * 옵션 설명 list  생성 
 * $result : $result->fields["name"] = "옵션명" 
 * $result : $result->fields["dscr"] = "설명" 
 * $result : $result->fields["opt_dscr_seqno"] = "옵션 설명 일련번호" 
 * 
 * return : list
 */
function makeOptDscrList($conn, $result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    while ($result && !$result->EOF) {

        $name = $result->fields["name"];
        $dscr = $result->fields["dscr"];
        $opt_dscr_seqno = $result->fields["opt_dscr_seqno"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n    <td>%d</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>"; 
        $list .= "\n    <td><button type=\"button\" onclick=\"popOptDscr(%d)\" class=\"orge btn_pu btn fix_height20 fix_width40\">수정</button></td>";

        $ret .= sprintf($list, $i, $name, $dscr, $opt_dscr_seqno); 

        $result->moveNext();
        $i++;
    }

    return $ret;
}


function makeDirectDlvrList($conn, $dao, $result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    while ($result && !$result->EOF) {
        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        //$list .= "\n    <td><button type=\"button\" onclick=\"popOptDscr(%d)\" class=\"orge btn_pu btn fix_height20 fix_width40\">수정</button></td>";
        $param["member_seqno"] = $result->fields["member_seqno"];
        $param["order_regi_date"] = date("Y-m");
        $pay_price1 = $dao->selectPayPrice($conn, $param);
        $param["order_regi_date"] = date("Y-m",strtotime("-1 month"));
        $pay_price2 = $dao->selectPayPrice($conn, $param);

        $ret .= sprintf($list, $i,
            "",
            $result->fields["member_name"],
            $result->fields["addr"] . " " . $result->fields["addr_detail"],
            explode(" ", $result->fields["order_regi_date"])[0],
            $result->fields["erp_state_name"],
            $pay_price1 == "" ? 0 : number_format($pay_price1),
            $pay_price2 == "" ? 0 : number_format($pay_price2)
            );

        $result->moveNext();
        $i++;
    }

    return $ret;
}


?>
