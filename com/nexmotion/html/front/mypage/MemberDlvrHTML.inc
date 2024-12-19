<?
//배송지 리스트
function makeDlvrListHtml($result, $param) {

    $ret = "";
    
    $list .= "\n  <tr>";
    $list .= "\n    <td><label class=\"check\"><input type=\"checkbox\" name=\"chk[]\" value=\"%s\" %s class=\"_individual\"><i><i></i></i></label></td>";
    $list .= "\n    <td>%s</td>";
    $list .= "\n    <td class=\"left\">%s</td>";
    $list .= "\n    <td class=\"left\">%s</td>";
    $list .= "\n    <td class=\"left\">%s</td>";
    $list .= "\n    <td>%s</td>";
    $list .= "\n    <td class=\"btn\">%s</td>";
    $list .= "\n  </tr>";

    while ($result && !$result->EOF) {

        $tel = $result->fields["tel_num"];
        $cell_num = $result->fields["cell_num"];
        if (strlen($cell_num) > 11) {
            $tel = $cell_num;
        }

        if (strlen($tel) < 10) {
            $tel = "";
        }

        $disabled = "";
        $btn  = "";
        $btn .= "<button type=\"button\" class=\"modify\" onclick=\"layerPopup('l_address', 'popup/l_address.html?seqno=%s'); return false;\" title=\"수정\">수정</button>";
        if ($result->fields["basic_yn"] != "Y") {
            $btn .= "<button type=\"button\" onclick=\"del('%s'); return false;\" class=\"cancel\" title=\"삭제\">삭제</button>";
        } else {
            $disabled = "disabled";
        }

        $btn = sprintf($btn, $result->fields["member_dlvr_seqno"]
                           , $result->fields["member_dlvr_seqno"]);

        $ret .= sprintf($list
                        ,$result->fields["member_dlvr_seqno"]
                        ,$disabled
                        ,date("Y-m-d", strtotime($result->fields["regi_date"]))
                        ,$result->fields["dlvr_name"]
                        ,$result->fields["recei"]
                        ,$result->fields["addr"]. " " .$result->fields["addr_detail"]
                        ,$tel
                        ,$btn);

        $result->moveNext();
    }

    return $ret;
}

//배송친구 메인 리스트
function makeDlvrMainList($rs) {

    $html  = "\n<tr>";
    $html .= "\n    <td><input type=\"radio\" name=\"friendCompany\" value=\"%s\"></td>";
    $html .= "\n    <th scope=\"row\">%s</th>";
    $html .= "\n    <td class=\"address\">%s %s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n</tr>";

    while ($rs && !$rs->EOF) {

        $ret .= sprintf($html, $rs->fields["dlvr_friend_main_seqno"]
                             , $rs->fields["office_nick"]
                             , $rs->fields["addr"]
                             , $rs->fields["addr_detail"]
                             , $rs->fields["tel_num"]);
        $rs->moveNext();
    }

    return $ret;
}
?>
