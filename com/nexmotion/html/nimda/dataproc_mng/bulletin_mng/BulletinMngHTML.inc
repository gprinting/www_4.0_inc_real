<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/com/nexmotion/doc/nimda/dataproc_mng/bulletin_mng/BulletinMngDOC.inc");

/* 
 * 팝업 list  생성 
 * $result : $result->fields["name"] = "팝업명" 
 * $result : $result->fields["post_start_date"] = "시작 일자" 
 * $result : $result->fields["post_end_date"] = "종료 일자" 
 * $result : $result->fields["start_hour"] = "시작 시간" 
 * $result : $result->fields["end_hour"] = "종료 시간" 
 * $result : $result->fields["popup_admin_seqno"] = "팝업 관리 일련번호" 
 * 
 * return : list
 */
function makePopupList($result) {

    $ret = "";
    $i = 1;

    while ($result && !$result->EOF) {

        $name = $result->fields["name"];
        $post_start_date = $result->fields["post_start_date"];
        $post_end_date = $result->fields["post_end_date"];
        $start_hour = substr($result->fields["start_hour"],0,5);
        $end_hour = substr($result->fields["end_hour"],0,5);
        if ($result->fields["use_yn"] == "Y") {

            $use_yn = "사용";

        } else {

            $use_yn = "미사용";

        }
        $popup_seqno = $result->fields["popup_admin_seqno"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n    <td>%d</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td><button type=\"button\" onclick=\"previewPopup(%d)\" class=\"green btn_pu btn fix_height20 fix_width40\">보기</button></td>";
        $list .= "\n    <td><button type=\"button\" onclick=\"popPopupSet(%d)\" class=\"orge btn_pu btn fix_height20 fix_width40\">수정</button></td>";
        $list .= "\n    </tr>";

        $ret .= sprintf($list, $i, $name, 
                $post_start_date . " ~ ". $post_end_date,
                $start_hour . "~" . $end_hour, 
                $use_yn, $popup_seqno, $popup_seqno); 

        $result->moveNext();
        $i++;
    }

    return $ret;
}

/* 
 * 공지 list  생성 
 * $result : $result->fields["title"] = "제목" 
 * $result : $result->fields["regi_date"] = "등록 일자" 
 * $result : $result->fields["hits"] = "조회수" 
 * $result : $result->fields["name"] = "작성자" 
 * $result : $result->fields["notice_seqno"] = "공지사항 일련번호" 
 * 
 * return : list
 */
function makeNoticeList($result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    while ($result && !$result->EOF) {

        $title = $result->fields["title"];
        $regi_date = $result->fields["regi_date"];
        $hits = number_format($result->fields["hits"]);
        $name = $result->fields["name"];
        $notice_seqno = $result->fields["notice_seqno"];

        if ($result->fields["dvs"] == 0) {
            $dvs = "없음";
        } else if ($result->fields["dvs"] == 1) {
            $dvs = "호환성문제";
        } else if ($result->fields["dvs"] == 2) {
            $dvs = "긴급";
        }

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n    <td>%d</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td><button type=\"button\" onclick=\"popNoticeSet(%d)\" class=\"green btn_pu btn fix_height20 fix_width40\">보기</button></td>";
        $list .= "\n    </tr>";

        $ret .= sprintf($list, $i, $dvs, $title, 
                $name, $regi_date, $hits, $notice_seqno); 

        $result->moveNext();
        $i++;
    }

    return $ret;
}


//공지사항 리스트 - 메인 요약 리스트
function makeNoticeSummary($rs) {
  
    if (!$rs) {
        return false;
    }

    $rs_html = "<table class=\"table_list\" height=\"130\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\"><tbody>";
    $html  = "\n    <tr><td><span class=\"board_list\" onclick=\"goCscenter('/cscenter/notice_view.html?seqno=%s');\">%s%s</span></td></tr> ";

    while ($rs && !$rs->EOF) {

        $dvs = $rs->fields["dvs"];
        if ($dvs == "" || $dvs == "0") {
            $dvs = "";
        } else if ($dvs == "1") {
            $dvs = "[호환성여부]";
        } else if ($dvs == "2") {
            $dvs = "[긴급]";
        }

        $rs_html .= sprintf($html,
                $rs->fields["notice_seqno"],
                $dvs,
                $rs->fields["title"]);
        
        $rs->moveNext();

    }

    $rs_html .= "</tbody></table>";

    return $rs_html;

}

/* 
 * FAQ list  생성 
 * $result : $result->fields["title"] = "제목" 
 * $result : $result->fields["regi_date"] = "등록 일자" 
 * $result : $result->fields["hits"] = "조회수" 
 * $result : $result->fields["name"] = "작성자" 
 * $result : $result->fields["faq_seqno"] = "FAQ 일련번호" 
 * 
 * return : list
 */
function makeFaqList($result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    while ($result && !$result->EOF) {

        $title = $result->fields["title"];
        $regi_date = $result->fields["regi_date"];
        $hits = number_format($result->fields["hits"]);
        $name = $result->fields["name"];
        $faq_seqno = $result->fields["faq_seqno"];
        $dvs  = $result->fields["sort"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n    <td>%d</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td><button type=\"button\" onclick=\"popFaqSet(%d)\" class=\"green btn_pu btn fix_height20 fix_width40\">보기</button></td>";
        $list .= "\n    </tr>";

        $ret .= sprintf($list, $i, $dvs, $title, 
                $regi_date, $hits, $faq_seqno); 

        $result->moveNext();
        $i++;
    }

    return $ret;
}





?>
