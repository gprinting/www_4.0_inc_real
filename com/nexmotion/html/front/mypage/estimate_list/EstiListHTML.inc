<?
function str_cut($str, $start = 0, $end, $tail = "..") {

    if (!$str) return "";

    if (strlen($str) > $end)
        return mb_substr($str, $start, $end, 'UTF-8') . $tail;
    else 
        return $str;
}

/**
 * @brief 견적 리스트 HTML
 */
function makeEstiListHtml($rs, $param) {
    if (!$rs) {
        return false;
    }

    $state_arr = $param["state_arr"];

    $rs_html = "";
    $html  = "\n  <tr class='%s'>"; //#1 class
    $html .= "\n    <td><label class=\"check\"><input type=\"checkbox\" class=\"_individual\" name=\"esti_seqno\" value=\"%s\"><i><i></i></i></label></td>"; //#2 esti_seqno
    $html .= "\n    <td>%s</td>"; //#3 regi_date
    $html .= "\n    <td class='subject'><div class=\"subject\"><span class=\"category_text\">[%s]</span> <span class=\"order_list_title_text\">%s</span><br><ul class=\"information\">%s</ul></div></td>"; //#4 cate_name, title, esti_detail
    $html .= "\n    <td class=\"price\">%s</td>"; //#5 esti_price
    $html .= "\n    <td>%s</td>"; //#6 esti_date
    $html .= "\n    <td>%s</td>"; //#7 esti_mng
    $html .= "\n    <td id=\"status_%s\" status=\"%s\"><span class=\"status no%s\">%s</span></td>"; //#8 esti_seqno, state
    $html .= "\n    <td class=\"btn menu\">";
    $html .= "\n        <button type=\"button\" class=\"menu\" onclick=\"$(this).parent().addClass('menuOn');\">메뉴 보기</button>";
    $html .= "\n        <button type=\"button\" class=\"menuClose\" onclick=\"$(this).parent().removeClass('menuOn');\">메뉴 보기</button>";
    $html .= "\n        <div class=\"menuWrap\">";
    $html .= "\n            <ul>";
    $html .= "\n                <li><button type=\"button\" onclick=\"showEstiPopMypage('%s');\">견적서 출력</button></li>";
    $html .= "\n                <li><button type=\"button\" onclick=\"goEstiOrder('%s')\">주문하기</button></li>"; //#9 esti_seqno
    $html .= "\n            </ul>";
    $html .= "\n        </div>";
    $html .= "\n    </td>";
    $html .= "\n  </tr>";
    $html .= "\n  <tr class='toggle_bot_%s hidden_row'></tr>"; //#10 esti_seqno
    $i = 1 + $param["s_num"];

    while ($rs && !$rs->EOF) {
        $fields = $rs->fields;

        if ($fields["state"] === $state_arr["견적완료"])
            $class = "";
        else 
            $class = "waiting";


        $esti_date = "-";
        if (!empty($fields["esti_date"])) {
            $esti_date = date("Y-m-d", strtotime($fields["esti_date"]));
        }

        $regi_date = "-";
        if (!empty($fields["regi_date"])) {
            $regi_date = date("Y-m-d", strtotime($fields["regi_date"]));
        }

        //상태별 이미지 필요
        switch ($fields["state"]) {
            case $state_arr["견적대기"] :
                $state = "견적대기";
                break;
            case $state_arr["견적중"] :
                $state = "견적중";
                break;
            case $state_arr["견적완료"] :
                $state = "견적완료";
                break;
        }

        $final_order_date = $fields["final_order_date"];

        $esti_detail = $fields["esti_detail"];
        $cate_name = trim(explode(',', (explode('/', $esti_detail)[0]))[0]);
        $esti_detail = explode('/', $esti_detail, 2)[1];

        $rs_html .= sprintf($html, $class //#1
                                 , $fields["esti_seqno"] //#2
                                 , $regi_date //#3
                                 , $cate_name //#4
                                 , $fields["title"] //#4
                                 , $esti_detail //#4
                                 , number_format($fields["order_price"]) . "원" //#5
                                 , $esti_date //#6
                                 , $fields["esti_mng"] //#7
                                 , $fields["esti_seqno"] //#8
                                 , $fields["state"] //#8
                                 , $fields["state"] //#8
                                 , $state //#8
                                 , $rs->fields["esti_seqno"] //#9
                                 , $rs->fields["esti_seqno"] //#9
                                 , $rs->fields["esti_seqno"] //#10
                                 );
        $i++;
        $rs->moveNext();
    }

    return $rs_html;
}
?>
