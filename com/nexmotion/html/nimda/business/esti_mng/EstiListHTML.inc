<?
/**
 * @brief 견적 리스트 HTML
 */
function makeEstiListHtml($rs, $param) {
  
    if (!$rs) {
        return false;
    }

    $today = date("Y-m-d");

    $rs_html = "";
    $html  = "\n  <tr class='%s'>";
    $html .= "\n    <td><input type=\"checkbox\" class=\"check_box\" name=\"chk\" value=\"%s\"></td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td>%s</td>";
    $html .= "\n    <td><button type=\"button\" class=\"orge btn_pu btn fix_height20 fix_width75\" onclick=\"getEsti.exec('%s');\">견적</button></td>";
    $html .= "\n  </tr>";
    $i = 1 + $param["s_num"];

    while ($rs && !$rs->EOF) {

        if ($i % 2 == 0) {
            $class = "cellbg";
        } else if ($i % 2 == 1) {
            $class = "";
        }

        if ($rs->fields["expec_order_date"] && 
                $today >= date("Y-m-d", strtotime($rs->fields["expec_order_date"] . "-3 day"))) {
            $class = "selbg";
        }

        $rs_html .= sprintf($html, $class, 
                $rs->fields["esti_seqno"],
                date("Y-m-d", strtotime($rs->fields["req_date"])),
                $rs->fields["member_name"] . " <span style=\"color:blue; font-weight: bold;\">[" . $rs->fields["office_nick"] . "]</span>",
                $rs->fields["esti_seqno"],
                $rs->fields["title"],
                number_format($rs->fields["esti_price"]) . "원",
                $rs->fields["state"],
                $rs->fields["esti_seqno"]);
        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}
?>
