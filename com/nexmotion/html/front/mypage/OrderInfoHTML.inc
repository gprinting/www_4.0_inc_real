<?
define(INC_PATH, $_SERVER["INC"]);
include_once(INC_PATH . "/common_define/common_info.inc");
include_once(INC_PATH . "/common_lib/CommonUtil.inc");
include_once(INC_PATH . "/com/nexmotion/job/front/mypage/OrderInfoDAO.inc");
include_once(INC_PATH . "/com/nexmotion/job/front/mypage/InterestDAO.inc");

//주문리스트
function makeOrderListHtml($conn, $rs, $param) {

    $util = new CommonUtil();
    $ret = "";
    $html  = "\n        <tbody class=\"multi\" name=\"order_list\">";
    $html .= "\n            <tr>";
    $html .= "\n                <td>%s<br>%s</td>"; // #1 주문번호
    $html .= "\n                <td>%s</td>"; // #2 주문일
    $html .= "\n                <td id=\"title_td_%s\" class=\"left\">";  // #3 td id
    $html .= "\n                    <div class=\"subject\">";
    //$html .= "\n                        <span class=\"category_text\">[%s]</span>&nbsp;<span class=\"order_list_title_text\">%s</span><br>"; // #4 카테고리, 제목
    $html .= "\n                        <span class=\"order_list_title_text\" onclick=\"layerPopup('l_order_detail', 'popup/l_order_detail_new.html?seqno=%s');\">%s</span><br>"; // #4 카테고리, 제목
    $html .= "\n                        <ul class=\"information\">";
    $html .= "\n                            %s"; // #5 세부정보
    $html .= "\n                        </ul>";
    $html .= "\n                        %s ";
    $html .= "\n                    </div>";
    $html .= "\n                </td>";
    //$html .= "\n                <td class=\"left\">%s</td>";    // #6 후가공
    $html .= "\n                <td class='amount'>%s%s%s</td>"; // #7 수량
    $html .= "\n                <td class='price'>%s원</td>"; // #8 가격
    $html .= "\n                <td %s><span class=\"status\">%s</span></td>";   // #9 진행상태
    $html .= "\n                <td class=\"btn menu\">";
    $html .= "\n                    <button id=\"btn_dt_%d\" class=\"menu viewOrderDetails _showOrderDetails _on\" title=\"상세보기\" onclick=\"openButtonBox('%d', '%s', this.id, '', '%s');\">상세보기</button>";
    $html .= "\n                    <button id=\"btn_dt_close_%d\" class=\"menuClose\" title=\"상세접기\" onclick=\"closeButtonBox('%s');\">상세접기</button>";
    $html .= "\n                    <div class=\"pollSlider _slide_%d\"></div>";
    $html .= "\n                </td>";
    $html .= "\n                <input type=\"hidden\" id=\"prevIdx\">";
    $html .= "\n            </tr>";
    $html .= "\n        </tbody>";

    $i = $param["count"] - $param["s_num"];
    $j = 1;

    $bun_group_prev = -1;

    while ($rs && !$rs->EOF) {
        if (empty($rs->fields["order_common_seqno"])) {
            $rs->MoveNext();
            continue;
        }

        if (!empty($rs->fields["amt"]))  {
            $amt = doubleval($rs->fields["amt"]);

            if ($amt < 1) {
                $amt = number_format($amt, 1);
            } else {
                $amt = number_format($amt);
            }
        } else {
            $amt = "";
        }

        $afterInfo = makeAfterHtml($conn, $rs->fields["order_common_seqno"], '');
        if($afterInfo != "") {
            $afterInfo = "후가공 : " . $afterInfo;
        }
        if (!empty($rs->fields["count"])) {
            $count = "(" . number_format($rs->fields["count"]) . "건" . ")";
        } else {
            $count = "";
        }

        //영수증
        $deal_num = $rs->fields["deal_num"];
        $deal_html = "";
        if($deal_num != "") {
            $deal_html = "<span class='status' onclick=\"showCardReceipt('".$deal_num."')\";>카드전표</span>";
        }

        $bun_group = $rs->fields["bun_group"];

        $pay_price = doubleval($rs->fields["pay_price"]);

        //*************************** 후공정 상태값 범위 후가공으로 퉁치기
        $order_state = $rs->fields["front_state_name"];
        if($order_state == "주문취소") $pay_price = 0;

        if($rs->fields["file_upload_dvs"] == "R") {
            $order_state = "재접수요청";
        }

        if($rs->fields["front_state_name"] == "접수대기" && $rs->fields["order_mng"] != "") {
            $order_state = "접수중";
        }

//front_state_name
        $css = '';
        if ($order_state === "주문취소") {
            $css = "class=\"cancel\"";
        } else if ($order_state === "접수대기") {
            $css = "class=\"wait\"";
        } else if ($order_state === "배송중") {
            $css = "class=\"departure\"";
        } else if ($order_state === "완료") {
            $css = "class=\"completed\"";
        } else {
            $css = "class=\"inProgress\"";
        }
        $title = $rs->fields["title"];
        if($rs->fields["rework_cause"] != null) {
            $title = "<span style='color:red; font-weight: bold;'>[" . $rs->fields["rework_cause"] . "] </span>" . $title;
        }

        $ret .= sprintf($html
            , substr($rs->fields["depo_finish_date"], 0,10) . "<br>" . substr($rs->fields["depo_finish_date"], 11)         // #2 주문일
            ,$deal_html
            , $rs->fields["order_num"] // #1 번호
            , $i
            , $rs->fields["order_common_seqno"]
            //, trim(explode(',', (explode('/', $rs->fields["order_detail"])[0]))[0]) // #4 카테고리
            , $title                              // #4 제목
            , explode('/', $rs->fields["order_detail"], 2)[1]      // #5 세부정보
            , $afterInfo                                          // #6 후공정
            , $amt                                                 // #7 수량
            , $rs->fields["amt_unit_dvs"]                          // #7 수량
            , $count                                               // #7 수량
            , number_format($pay_price)                            // #8 가격
            , $css                                                 // #9
            , $order_state                                         // #9 진행상태
            , $i                                                   // #10 버튼오픈
            , $i                                                   // #10 버튼오픈
            , $rs->fields["order_common_seqno"]                    // #10 버튼오픈
            , intval($rs->fields["order_state"])                   // #10-J 버튼 파라미터(진행상태)
            , $i                                                   // #11 버튼닫음
            , $i                                                   // #12 마스크
            , $i                                                   // #12 마스크
            , $i);                                                 // #13 버튼모음
        $bun_group_prev = $rs->fields["bun_group"];
        $i--;
        $rs->moveNext();
    }

    if ($rs->recordCount() == 0) {
        $ret  = "\n        <tbody class=\"multi\" name=\"order_list\">";
        $ret .= "\n            <tr>";
        $ret .= "\n                <td colspan=\"7\">주문이 없습니다.</td>";
        $ret .= "\n            </tr>";
        $ret .= "\n        </tbody>";
    }

    return $ret;
}

function makeOrderListHtml2($conn, $rs, $param) {

    $util = new CommonUtil();
    $ret = "";
    $html  = "\n        <tbody class=\"multi\" name=\"order_list\">";
    $html .= "\n            <tr>";
    $html .= "\n                <td>%s</td>"; // #1 번호
    $html .= "\n                <td>%s</td>"; // #2 주문번호
    $html .= "\n                <td>%s%s</td>"; // #3 주문일
    $html .= "\n                <td id=\"title_td_%s\" class=\"left\">";  // #4 td id
    $html .= "\n                    <div class=\"subject\">";
    //$html .= "\n                        <span class=\"category_text\">[%s]</span>&nbsp;<span class=\"order_list_title_text\">%s</span><br>"; // #4 카테고리, 제목
    $html .= "\n                        <span class=\"order_list_title_text\" onclick=\"layerPopup('l_order_detail', 'popup/l_order_detail_new.html?seqno=%s');\">%s</span><br>"; // #5 카테고리, 제목
    $html .= "\n                        <ul class=\"information\">";
    $html .= "\n                            %s"; // #6 세부정보
    $html .= "\n                        </ul>";
    $html .= "\n                        %s ";
    $html .= "\n                    </div>";
    $html .= "\n                </td>";
    //$html .= "\n                <td class=\"left\">%s</td>";    // #7 후가공
    $html .= "\n                <td class='amount'>%s%s%s</td>"; // #8 수량
    $html .= "\n                <td class='price'>%s원</td>"; // #9 가격
    $html .= "\n                <td %s><span class=\"status\">%s</span></td>";   // #10 진행상태
    $html .= "\n                <td>%s</td>";   // #9 배송유형
    $html .= "\n                <td class=\"btn menu\">";
    $html .= "\n                    <button id=\"btn_dt_%d\" class=\"menu viewOrderDetails _showOrderDetails _on\" title=\"상세보기\" onclick=\"openButtonBox('%d', '%s', this.id, '', '%s');\">상세보기</button>";
    $html .= "\n                    <button class=\"menuClose\" title=\"상세접기\" onclick=\"closeButtonBox('%s');\">상세접기</button>";
    $html .= "\n                    <div class=\"menuWrap _slide_%d\"></div>";
    $html .= "\n                </td>";
    $html .= "\n                <input type=\"hidden\" id=\"prevIdx\">";
    $html .= "\n            </tr>";
    $html .= "\n        </tbody>";

    $i = $param["count"] - $param["s_num"];
    $j = 1;

    $bun_group_prev = -1;

    while ($rs && !$rs->EOF) {
        if (empty($rs->fields["order_common_seqno"])) {
            $rs->MoveNext();
            continue;
        }

        if (!empty($rs->fields["amt"]))  {
            $amt = doubleval($rs->fields["amt"]);

            if ($amt < 1) {
                $amt = number_format($amt, 1);
            } else {
                $amt = number_format($amt);
            }
        } else {
            $amt = "";
        }

        $afterInfo = makeAfterHtml($conn, $rs->fields["order_common_seqno"], '');
        if($afterInfo != "") {
            $afterInfo = "후가공 : " . $afterInfo;
        }
        if (!empty($rs->fields["count"])) {
            $count = "(" . number_format($rs->fields["count"]) . "건" . ")";
        } else {
            $count = "";
        }

        //영수증
        $deal_num = $rs->fields["deal_num"];
        $deal_html = "";
        if($deal_num != "") {
            $deal_html = "<span class='status' onclick=\"showCardReceipt('".$deal_num."')\";>카드전표</span>";
        }

        $bun_group = $rs->fields["bun_group"];

        $pay_price = doubleval($rs->fields["pay_price"]);

        //*************************** 후공정 상태값 범위 후가공으로 퉁치기
        $order_state = $rs->fields["front_state_name"];
        if($order_state == "주문취소") $pay_price = 0;

        if($rs->fields["file_upload_dvs"] == "R") {
            $order_state = "재접수요청";
        }

        if($rs->fields["front_state_name"] == "접수대기" && $rs->fields["order_mng"] != "") {
            $order_state = "접수중";
        }

	//front_state_name
        $css = '';
        if ($order_state === "주문취소") {
            $css = "class=\"cancel\"";
        } else if ($order_state === "접수대기") {
            $css = "class=\"wait\"";
        } else if ($order_state === "배송중") {
            $css = "class=\"departure\"";
        } else if ($order_state === "완료") {
            $css = "class=\"completed\"";
        } else {
            $css = "class=\"inProgress\"";
        }
        $title = $rs->fields["title"];
        if($rs->fields["rework_cause"] != null) {
            $title = "<span style='color:red; font-weight: bold;'>[" . $rs->fields["rework_cause"] . "] </span>" . $title;
        }

        // 배송정보

        $dlvr_way = "";
            if ($rs->fields["dlvr_way"] == "01") {
                if($rs->fields["dlvr_sum_way"] == "01") {
                    $dlvr_way = "선불택배";
                } else if ($rs->fields["dlvr_sum_way"] == "02") {
                    $dlvr_way = "착불택배";
                } else {
                    $dlvr_way = "택배";
                }
            } else if ($rs->fields["dlvr_way"] == "02"){
                $dlvr_way = "직배";
            } else if ($rs->fields["dlvr_way"] == "03"){
                $dlvr_way = "화물";
            } else if ($rs->fields["dlvr_way"] == "04"){
                $dlvr_way = "퀵";
            } else if ($rs->fields["dlvr_way"] == "05"){
                $dlvr_way = "퀵";
            } else if ($rs->fields["dlvr_way"] == "06"){
                $dlvr_way = "방문(인현동)";
            } else if ($rs->fields["dlvr_way"] == "07"){
                $dlvr_way = "방문(성수동)";
            }


        $ret .= sprintf($html
	    , $i // #1 번호
	    , substr($rs->fields["depo_finish_date"], 0,10) . "<br>" . substr($rs->fields["depo_finish_date"], 11)         // #2 주문일
            , $rs->fields["order_num"] // #1 번호
	    ,$deal_html
            , $i
            , $rs->fields["order_common_seqno"]
            //, trim(explode(',', (explode('/', $rs->fields["order_detail"])[0]))[0]) // #4 카테고리
            , $title                              // #4 제목
            , explode('/', $rs->fields["order_detail"], 2)[1]      // #5 세부정보
            , $afterInfo        // #6 후공정
            , $amt                                                 // #7 수량
            , $rs->fields["amt_unit_dvs"]                          // #7 수량
            , $count                                               // #7 수량
            , number_format($pay_price)                            // #8 가격
            , $css                                                 // #9
            , $order_state                                         // #9 진행상태
            , $dlvr_way                                            // #9 진행상태
            , $i                                                   // #10 버튼오픈
            , $i                                                   // #10 버튼오픈
            , $rs->fields["order_common_seqno"]                    // #10 버튼오픈
            , intval($rs->fields["order_state"])                   // #10-J 버튼 파라미터(진행상태)
            , $i                                                   // #11 버튼닫음
            , $i                                                   // #12 마스크
            , $i);                                                 // #13 버튼모음
        $bun_group_prev = $rs->fields["bun_group"];
        $i--;
        $rs->moveNext();
    }

    if ($rs->recordCount() == 0) {
        $ret  = "\n        <tbody class=\"multi\" name=\"order_list\">";
        $ret .= "\n            <tr>";
        $ret .= "\n                <td colspan=\"7\">주문이 없습니다.</td>";
        $ret .= "\n            </tr>";
        $ret .= "\n        </tbody>";
    }

    return $ret;
}



//기업회원을 제외한 모바일 주문리스트
function makeOrderListHtmlMobile($conn, $rs, $param) {

    $util = new CommonUtil();

    $ret = "";
    $html  = "\n        <tbody class=\"multi\" name=\"order_list\">";
    $html .= "\n            <tr>";
    $html .= "\n                <td>%s</td>";   // #1. 주문일
    $html .= "\n                <td class=\"subject\">%s</td>"; // #2. 제목
    $html .= "\n                <td><span class=\"status\">%s</span></td>";   // #3. 진행상태
    $html .= "\n                <td class=\"btn detail\">";
    $html .= "\n                    <div class=\"btn_board_list_manage\">";
    $html .= "\n                    <button type=\"button\" class=\"menu\" id=\"det_op_%d\" onclick=\"openOrderDetail('%d', '%s');\">상세보기</button>";   // #4. 상세보기 펴기
    $html .= "\n                    <button type=\"button\" class=\"menuClose\" id=\"det_cl_%d\" onclick=\"closeOrderDetail('%s');\">상세접기</button>";    // #5. 상세보기 접기
    $html .= "\n                </td>";
    $html .= "\n            </tr>";
    $html .= "\n            <tr class=\"_orderDetails\" id=\"detail%d\">";  // #6. 상세보기 내용
    $html .= "\n            </tr>";
    $html .= "\n        </tbody>";

    $i = $param["count"] - $param["s_num"];

    while ($rs && !$rs->EOF) {
        if (empty($rs->fields["order_common_seqno"])) {
            $rs->MoveNext();
            continue;
        }

        if (!empty($rs->fields["count"])) {
            $count = "(" . number_format($rs->fields["count"]) . ")";
        } else {
            $count = "";
        }

        $ret .= sprintf($html, substr($rs->fields["order_regi_date"], 0,10)     // #1. 주문일
            , $rs->fields["title"]   // #2. 제목
            , $util->statusCode2status($rs->fields["order_state"]) // #3. 진행상태
            , $i   // #4. 상세보기 펴기
            , $i   // #4. 상세보기 펴기
            , $rs->fields["order_common_seqno"]      // #4. 상세보기 펴기
            , $i   // #5. 상세보기 접기
            , $i   // #5. 상세보기 접기
            , $i); // #6. 상세보기 내용

        $i--;
        $rs->moveNext();
    }

    if ($rs->recordCount() == 0) {
        $ret  = "\n        <tbody class=\"multi\" name=\"order_list\">";
        $ret .= "\n            <tr>";
        $ret .= "\n                <td colspan=\"7\">주문이 없습니다.</td>";
        $ret .= "\n            </tr>";
        $ret .= "\n        </tbody>";
    }

    return $ret;

}

// 기업회원을 제외한 마이페이지 메인리스트
function makeOrderListMainHtml($rs, $param) {

    $util = new CommonUtil();

    $ret = "";
    $html  = "\n        <tbody class=\"multi\" name=\"order_list\">";
    $html .= "\n            <tr>";
    $html .= "\n                <td>%s</td>"; // #1 번호
    $html .= "\n                <td>%s</td>"; // #2 주문일
    $html .= "\n                <td id=\"title_td_%s\" class=\"left\">";  // #3 td id
    $html .= "\n                    <div class=\"subject\">";
    $html .= "\n                        <span class=\"category_text\">[%s]</span>&nbsp;<span class=\"order_list_title_text\">%s</span><br>"; // #4 카테고리, 제목
    $html .= "\n                        <ul class=\"information\">";
    $html .= "\n                            %s"; // #5 세부정보
    $html .= "\n                        </ul>";
    $html .= "\n                    </div>";
    $html .= "\n                </td>";
    $html .= "\n                <td class=\"left\">%s</td>";    // #6 후가공
    $html .= "\n                <td class=\"amount\">%s%s%s</td>"; //오류나서 임시로 값지정 기존값 %s%s%s #7 수량
    $html .= "\n                <td class='price'>%s원</td>"; // #8 가격
    $html .= "\n                <td><span class=\"status\">%s</span></td>";   // #9 진행상태
    $html .= "\n                <td class=\"btn menu\">";
    $html .= "\n                    <button id=\"btn_dt_%d\" class=\"menu viewOrderDetails _showOrderDetails _on\" title=\"상세보기\" onclick=\"openButtonBox('%d', '%s', this.id, 'main');\">상세보기</button>";
    $html .= "\n                    <button class=\"menuClose\" title=\"상세접기\" onclick=\"closeButtonBox('%s');\">상세접기</button>";
    $html .= "\n                    <div class=\"menuWrap _slide_%d\"></div>";
    $html .= "\n                </td>";
    $html .= "\n            </tr>";
    $html .= "\n        </tbody>";

    $i = $param["count"] - $param["s_num"];

    while ($rs && !$rs->EOF) {
        if (empty($rs->fields["order_common_seqno"])) {
            $rs->MoveNext();
            continue;
        }

        if (!empty($rs->fields["amt"]))  {
            $amt = doubleval($rs->fields["amt"]);

            if ($amt < 1) {
                $amt = number_format($amt, 1);
            } else {
                $amt = number_format($amt);
            }
        } else {
            $amt = "";
        }

        if (!empty($rs->fields["count"])) {
            $count = "(" . number_format($rs->fields["count"]) . ")";
        } else {
            $count = "";
        }

        $pay_price  = doubleval($rs->fields["pay_price"]);

        $ret .= sprintf($html, $i                                                   // #1 번호
            , substr($rs->fields["order_regi_date"], 0,10)         // #2 주문일
            , $i                                                   // #3 td id
            , explode(',', (explode('/', $rs->fields["order_detail"])[0]))[0] // #4 카테고리
            , $rs->fields["title"]                                 // #4 제목
            , $rs->fields["order_detail"]                          // #5 세부정보
            , makeAfterHtml($conn, $rs->fields["order_common_seqno"], '') // #6 후공정
            , $amt                                                 // #7 수량
            , $rs->fields["amt_unit_dvs"]                          // #7 수량
            , $count                                               // #7 수량
            , number_format($pay_price)                            // #8 가격
            , $util->statusCode2status($rs->fields["order_state"]) // #9 진행상태
            , $i                                                   // #10 버튼오픈
            , $i                                                   // #10 버튼오픈
            , $rs->fields["order_common_seqno"]                    // #10 버튼오픈
            , $i                                                   // #11 버튼닫음
            , $i                                                   // #12 마스크
            , $i);                                                 // #13 버튼모음
        $i--;
        $rs->moveNext();
    }

    if ($rs->recordCount() == 0) {
        $ret  = "\n        <tbody class=\"multi\" name=\"order_list\">";
        $ret .= "\n            <tr>";
        $ret .= "\n                <td colspan=\"8\">주문이 없습니다.</td>";
        $ret .= "\n            </tr>";
        $ret .= "\n        </tbody>";
    }

    return $ret;
}

//기업회원을 제외한 주문취소리스트
function makeOrderCancelListHtml($rs, $param) {

    $util = new CommonUtil();

    $ret = "";
    $html  = "\n        <tbody class=\"multi\" name=\"order_list\">";
    $html .= "\n            <tr>";
    $html .= "\n                <td>%s</td>"; // #1 번호
    $html .= "\n                <td>%s</td>"; // #2 주문일
    $html .= "\n                <td id=\"title_td_%s\" class=\"left\">";  // #3 td id
    $html .= "\n                    <div class=\"subject\">";
    $html .= "\n                        <span class=\"category_text\">[%s]</span>&nbsp;<span class=\"order_list_title_text\">%s</span><br>"; // #4 카테고리, 제목
    $html .= "\n                        <ul class=\"information\">";
    $html .= "\n                            %s"; // #5 세부정보
    $html .= "\n                        </ul>";
    $html .= "\n                    </div>";
    $html .= "\n                </td>";
    $html .= "\n                <td class=\"left\">%s</td>";    // #6 후가공
    $html .= "\n                <td class=\"right\">%s%s%s</td>"; //오류나서 임시로 값지정 기존값 %s%s%s #7 수량
    $html .= "\n                <td class='price'>%s원</td>"; // #8 가격
    $html .= "\n                <td><span class=\"status\">%s</span></td>";   // #9 진행상태
    $html .= "\n                <td class=\"btn menu\">";
    $html .= "\n                    <button id=\"btn_dt_%d\" class=\"menu viewOrderDetails _showOrderDetails _on\" title=\"상세보기\" onclick=\"openButtonBox('%d', '%s', this.id, '', '%s');\">상세보기</button>";
    $html .= "\n                    <button class=\"menuClose\" title=\"상세접기\" onclick=\"closeButtonBox('%s');\">상세접기</button>";
    $html .= "\n                    <div class=\"menuWrap _slide_%d\"></div>";
    $html .= "\n                </td>";
    $html .= "\n                <input type=\"hidden\" id=\"prevIdx\">";
    $html .= "\n            </tr>";
    $html .= "\n        </tbody>";

    $i = $param["count"] - $param["s_num"];

    while ($rs && !$rs->EOF) {
        if (empty($rs->fields["order_common_seqno"])) {
            $rs->MoveNext();
            continue;
        }

        if (!empty($rs->fields["amt"]))  {
            $amt = doubleval($rs->fields["amt"]);

            if ($amt < 1) {
                $amt = number_format($amt, 1);
            } else {
                $amt = number_format($amt);
            }
        } else {
            $amt = "";
        }

        if (!empty($rs->fields["count"])) {
            $count = "(" . number_format($rs->fields["count"]) . ")";
        } else {
            $count = "";
        }

        $pay_price  = doubleval($rs->fields["pay_price"]);

        $ret .= sprintf($html, $i                                                   // #1 번호
            , substr($rs->fields["order_regi_date"], 0,10)        // #2 주문일 
            , $i                                                   // #3 td id
            , trim(explode(',', (explode('/', $rs->fields["order_detail"])[0]))[0]) // #4 카테고리
            , $rs->fields["title"]                                 // #4 제목
            , explode('/', $rs->fields["order_detail"], 2)[1]      // #5 세부정보
            , makeAfterHtml($conn, $rs->fields["order_common_seqno"], '') // #6 후공정
            , $amt                                                 // #7 수량
            , $rs->fields["amt_unit_dvs"]                          // #7 수량
            , $count                                               // #7 수량
            , number_format($pay_price)                            // #8 가격
            , $util->statusCode2status($rs->fields["order_state"]) // #9 진행상태
            , $i                                                   // #10 버튼오픈
            , $i                                                   // #10 버튼오픈
            , $rs->fields["order_common_seqno"]                    // #10 버튼오픈
            , intval($rs->fields["order_state"])                   // #10-J 버튼 파라미터(진행상태)
            , $i                                                   // #11 버튼닫음
            , $i                                                   // #12 마스크
            , $i);                                                 // #13 버튼모음

        $i--;
        $rs->moveNext();
    }

    if ($rs->recordCount() == 0) {
        $ret  = "\n        <tbody class=\"multi\" name=\"order_list\">";
        $ret .= "\n            <tr>";
        $ret .= "\n                <td colspan=\"8\">주문이 없습니다.</td>";
        $ret .= "\n            </tr>";
        $ret .= "\n        </tbody>";
    }

    return $ret;
}



//기업회원 주문리스트
function makeBusiOrderListHtml($rs, $param) {

    $util = new CommonUtil();

    $ret = "";
    $html  = "\n        <tbody class=\"multi\" name=\"order_list\">";
    $html .= "\n            <tr>";
    $html .= "\n                <td class=\"delivery_grouup_color01\">";
    $html .= "\n                    <input type=\"checkbox\" name=\"seq\" class=\"_individual\" value=\"%s\" style=\"float:right;\">";
    $html .= "\n                </td>"; //#0 주문공통일련번호
    $html .= "\n                <td>%s</td>"; // #1 번호
    $html .= "\n                <td>%s</td>"; // #2 주문일
    $html .= "\n                <td>%s</td>"; // #2-1 주문자
    $html .= "\n                <td id=\"title_td_%s\" class=\"left\">";  // #3 td id
    $html .= "\n                    <div class=\"subject\">";
    $html .= "\n                        %s<br>"; // #4 제목
    $html .= "\n                        <ul class=\"information\">";
    $html .= "\n                            %s"; // #5 세부정보
    $html .= "\n                        </ul>";
    $html .= "\n                    </div>";
    $html .= "\n                </td>";
    $html .= "\n                <td class=\"left\">%s</td>";    // #6 후가공
    $html .= "\n                <td class=\"right\">%s%s%s</td>"; //오류나서 임시로 값지정 기존값 %s%s%s #7 수량
    $html .= "\n                <td class='price'>%s원</td>"; // #8 가격
    $html .= "\n                <td><span class=\"status\">%s</span></td>";   // #9 진행상태
    $html .= "\n                <td class=\"btn menu\">";
    $html .= "\n                    <button id=\"btn_dt_%d\" class=\"menu viewOrderDetails _showOrderDetails _on\" title=\"상세보기\" onclick=\"openButtonBox('%d', '%s', this.id);\">상세보기</button>";
    $html .= "\n                    <button class=\"menuClose\" title=\"상세접기\" onclick=\"closeButtonBox('%s');\">상세접기</button>";
    $html .= "\n                    <div class=\"menuWrap _slide_%d\"></div>";
    $html .= "\n                </td>";
    $html .= "\n            </tr>";
    $html .= "\n        </tbody>";

    $i = $param["count"] - $param["s_num"];

    while ($rs && !$rs->EOF) {
        if (empty($rs->fields["order_common_seqno"])) {
            $rs->MoveNext();
            continue;
        }

        if (!empty($rs->fields["amt"]))  {
            $amt = doubleval($rs->fields["amt"]);

            if ($amt < 1) {
                $amt = number_format($amt, 1);
            } else {
                $amt = number_format($amt);
            }
        } else {
            $amt = "";
        }

        if (!empty($rs->fields["count"])) {
            $count = "(" . number_format($rs->fields["count"]) . ")";
        } else {
            $count = "";
        }

        $pay_price  = doubleval($rs->fields["pay_price"]);
        $dlvr_price = doubleval($rs->fields["dlvr_price"]);

        $ret .= sprintf($html, $rs->fields["order_common_seqno"]                    // #0 주문공통일련번호
            , $i                                                   // #1 번호
            , substr($rs->fields["order_regi_date"], 0,10)         // #2 주문일
            , $rs->fields["member_name"]                           // #2-1 주문자
            , $i                                                   // #3 td id
            , $rs->fields["title"]                                 // #4 제목
            , $rs->fields["order_detail"]                          // #5 세부정보
            , "505"                                                // #6 후공정
            , $amt                                                 // #7 수량
            , $rs->fields["amt_unit_dvs"]                          // #7 수량
            , $count                                               // #7 수량
            , number_format($pay_price)                            // #8 가격
            , $util->statusCode2status($rs->fields["order_state"]) // #9 진행상태
            , $i                                                   // #10 버튼오픈
            , $i                                                   // #10 버튼오픈
            , $rs->fields["order_common_seqno"]                    // #10 버튼오픈
            , $i                                                   // #11 버튼닫음
            , $i                                                   // #12 마스크
            , $i);                                                 // #13 버튼모음
        $i--;
        $rs->moveNext();
    }

    return $ret;
}

//모바일 주문상세
function makeOrderdetailMobile($param, $util, $opt_rs, $aft_rs, $file_rs) {
    $conn = $param["conn"];
    $dao  = $param["dao"];
    $btn_flag = $param["btn_flag"];
    $order_state_arr = $param["order_state_arr"];

    // 17-07-07 값 비었으면 html 생성 안해야되서 위로 뺌
    $member_sale_price = doubleval($param["member_sale_price"]);

    $order_common_seqno = $param["order_common_seqno"];
    $order_state        = $param["order_state"];
    $dlvr_way           = DLVR_TYP[$param["dlvr_way"]];

    $html .= "\n <td colspan=\"" . $param["colspan"] . "\" class=\"board_type_detail_td detail_table_01\" style=\"padding: 0px !important;\">"; //#1 colspan
    $detail_base  = "\n     <div class=\"mypage_detail_table_wrapper\">";
    $detail_base .= "\n         <table class=\"mypage_detail_table\">";
    $detail_base .= "\n             <colgroup>";
    $detail_base .= "\n                 <col width=\"30%%;\">";
    $detail_base .= "\n                 <col>";
    $detail_base .= "\n             </colgroup>";
    $detail_base .= "\n             <tbody>";
    $detail_base .= "\n                 <tr>";
    $detail_base .= "\n                     <th>상품내역</th>";
    $detail_base .= "\n                     <td class=\"bottom_border\">%s</td>"; //#1 order_detail
    $detail_base .= "\n                 </tr>";
    $detail_base .= "\n                 <tr>";
    $detail_base .= "\n                     <th>옵션</th>";
    $detail_base .= "\n                     <td class=\"bottom_border\">%s</td>"; //#2-1 option
    $detail_base .= "\n                 </tr>";
    $detail_base .= "\n                 <tr>";
    $detail_base .= "\n                     <th>후공정</th>";
    $detail_base .= "\n                     <td class=\"bottom_border\">%s</td>"; //#2-2 after
    $detail_base .= "\n                 </tr>";
    $detail_base .= "\n                 <tr>";
    $detail_base .= "\n                     <th>주문파일</th>";
    $detail_base .= "\n                     <td class=\"bottom_border\">%s</td>"; //#2-3 order_file
    $detail_base .= "\n                 </tr>";
    $detail_base .= "\n                 <tr>";
    $detail_base .= "\n                     <th>수량/건</th>";
    $detail_base .= "\n                     <td class=\"bottom_border\">%s%s X %s건</td>"; //#3 amt, amt_unit_dvs, count
    $detail_base .= "\n                 </tr>";
    $detail_base .= "\n                 <tr>";
    $detail_base .= "\n                     <th>할인내역</th>";
    $detail_base .= "\n                     <td class=\"bottom_border\">회원등급할인 %s원</td>"; //#4 member_grade_sale, 차후에 이벤트랑 포인트 추가 필요
    if (!empty($member_sale_price)) {
        $detail_base .= "\n                 <td class=\"bottom_border\" style=\"padding-left:10px;\">회원특별할인 " . number_format($member_sale_price) . "원</td>";
    }
    $detail_base .= "\n                 </tr>";
    $detail_base .= "\n                 <tr>";
    $detail_base .= "\n                     <th>예상무게</th>";
    $detail_base .= "\n                     <td class=\"bottom_border\">%sKg</td>"; //#5 expec_weight
    $detail_base .= "\n                 </tr>";
    $detail_base .= "\n                 <tr>";
    /*
    $detail_base .= "\n                     <th>배송</th>";
    $detail_base .= "\n                     <td class=\"bottom_border\">[%s] %s %s %s</td>"; //#6 dlvr_way, zipcode, addr, addr_detail
    $detail_base .= "\n                 </tr>";
    */
    $detail_base .= "\n                 <tr>";
    $detail_base .= "\n                     <th>결제금액</th>";
    $detail_base .= "\n                     <td class=\"bottom_border\">판매가 %s원 + 후공정 %s원 + 옵션 %s원 + 배송비 %s원</td>"; //#7 pay_price
    $detail_base .= "\n                 </tr>";
    $detail_base .= "\n             </tbody>";
    $detail_base .= "\n         </table>";
    $detail_base .= "\n     </div>";

    $after_li_base = "\n            <li>%s</li>";

    $file_base = "\n                 <span onclick=\"downOrderFile('%s', '%s');\" style=\"cursor:pointer;\">%s<span><br/>";

    $after_html = '';
    while ($aft_rs && !$aft_rs->EOF) {
        $fields = $aft_rs->fields;

        // 16-12-17 ujh 기본후공정 안보여주게 수정
        if ($fields["basic_yn"] === 'Y') {
            $aft_rs->MoveNext();
            continue;
        }

        $aft = sprintf("%s(%s)", $fields["after_name"]
            , $util->getOptAfterFullName($fields));

        $after_html .= sprintf($after_li_base, $aft);

        $aft_rs->MoveNext();
    }

    $opt_html = '';
    while ($opt_rs && !$opt_rs->EOF) {
        $fields = $opt_rs->fields;

        // 16-12-17 ujh 기본옵션 안보여주게 수정
        if ($fields["basic_yn"] === 'Y') {
            $opt_rs->MoveNext();
            continue;
        }

        $opt = sprintf("%s(%s)", $fields["opt_name"]
            , $util->getOptAfterFullName($fields));

        $opt_html .= sprintf($after_li_base, $opt);

        $opt_rs->MoveNext();
    }

    $file_html = '';
    while ($file_rs && !$file_rs->EOF) {
        $fields = $file_rs->fields;

        $file_html .= sprintf($file_base, $order_common_seqno
            , $fields["order_file_seqno"]
            , $fields["origin_file_name"]);

        $file_rs->MoveNext();
    }

    $amt = doubleval($param["amt"]);
    $count = number_format(doubleval($param["count"]));

    if ($amt < 1) {
        $amt = number_format($amt, 1);
    } else {
        $amt = number_format($amt);
    }


    $sell_price        = doubleval($param["sell_price"]);
    $grade_sale_price  = doubleval($param["grade_sale_price"]);

    $pay_price        = number_format(doubleval($param["pay_price"]));
    $event_price      = number_format(doubleval($param["event_price"]));
    $use_point_price  = number_format(doubleval($param["use_point_price"]));
    $add_after_price  = number_format(doubleval($param["add_after_price"]));
    $add_opt_price    = number_format(doubleval($param["add_opt_price"]));
    $dlvr_price       = number_format(doubleval($param["dlvr_price"]));

    $html .= sprintf($detail_base, $param["order_detail"] //#1
        , $opt_html //#2-1
        , $after_html //#2-2
        , $file_html //#2-3
        , $amt //#3
        , $param["amt_unit_dvs"] //#3
        , $count //#3
        , number_format($grade_sale_price) //#4
        , $param["expec_weight"] //#5
        //     , $dlvr_way //#6
        //     , $param["zipcode"] //#6
        //     , $param["addr"] //#6
        //     , $param["addr_detail"] //#6
        , number_format($sell_price + $grade_sale_price + $member_sale_price) //#7
        , $add_after_price //#7
        , $add_opt_price   //#7
        , $dlvr_price  //#7
    );

    //$html .= "\n     <div class=\"threeColumn_btn_wrapper\" style=\"margin-bottom: 0;\">";
    /*
    $html .= "\n         <dl>";
    $html .= "\n             <dt>공통할인내역</dt>";
    $html .= "\n             <dd>이벤트할인 %s</dd>";
    $html .= "\n         </dl>";
    */

    $btn_base  = "\n         <table class=\"table_mypage_btn\">";
    $btn_base .= "\n             <colgroup>";
    $btn_base .= "\n                <col style=\"33%%;\">";
    $btn_base .= "\n                <col style=\"33%%;\">";
    $btn_base .= "\n                <col style=\"33%%;\">";
    $btn_base .= "\n             </colgroup>";
    $btn_base .= "\n             <tbody>";
    $btn_base .= "\n                <tr>";
    $btn_base .= "\n                    <td class=\"td_mypage_btn_orderCancel btn_orderCancel\" onclick=\"showOrderCancelPop('%s');\">"; //#1 order_common_seqno
    $btn_base .= "\n                        <img src=\"/design_template/m/images/mypage/btn_mypage_ordercancel.png\" alt=\"주문취소\">";
    $btn_base .= "\n                        <p>주문취소</p>";
    $btn_base .= "\n                    </td>";
    $btn_base .= "\n                    <td class=\"td_mypage_btn_sampleView btn_sampleView\" onclick=\"showDraftPop('%s');\">"; //#2 order_detail_seqno
    $btn_base .= "\n                        <img src=\"/design_template/m/images/mypage/btn_mypage_view.png\" alt=\"시안보기\">";
    $btn_base .= "\n                        <p>시안보기</p>";
    $btn_base .= "\n                    </td>";
    $btn_base .= "\n                    <td class=\"td_mypage_btn_delivery btn_delivery\">";
    $btn_base .= "\n                        <img src=\"/design_template/m/images/mypage/btn_mypage_delivery.png\" alt=\"배송조회\">";
    $btn_base .= "\n                        <p>배송조회</p>";
    $btn_base .= "\n                    </td>";
    $btn_base .= "\n                </tr>";
    $btn_base .= "\n                <tr>";
    //$btn_base .= "\n               <button onclick=\"showReOrderPop('%s');\"><img src=\"/design_template/images/mypage/btn_text_reorder.png\" alt=\"재주문\"></button>"; //#3 order_common_seqno
    $btn_base .= "\n                    <td class=\"td_mypage_btn_fileUpload btn_file_upload\" onclick=\"showReUploadPop('%s', '%s');\">"; //#4 order_common_seqno, order_state
    $btn_base .= "\n                        <img src=\"/design_template/m/images/mypage/btn_mypage_file.png\" alt=\"파일재전송\">";
    $btn_base .= "\n                        <p>파일재전송</p>";
    $btn_base .= "\n                    </td>";
    $btn_base .= "\n                    <td class=\"td_mypage_btn_claim btn_claim\" onclick=\"reqClaim('%s', '%s');\">"; //#5 order_common_seqno, order_state
    $btn_base .= "\n                        <img src=\"/design_template/m/images/mypage/btn_mypage_claim.png\" alt=\"클레임요청\">";
    $btn_base .= "\n                        <p>클레임요청</p>";
    $btn_base .= "\n                    </td>";
    $btn_base .= "\n                    <td class=\"td_mypage_btn_memo btn_memo\" onclick=\"showOrderMemoPop('%s');\">";
    $btn_base .= "\n                        <img src=\"/design_template/m/images/mypage/btn_mypage_memo.png\" alt=\"메모\">";
    $btn_base .= "\n                        <p>메모</p>";
    $btn_base .= "\n                    </td>";
    $btn_base .= "\n                </tr>";
    $btn_base .= "\n            </tbody>";
    $btn_base .= "\n        </table>";

    if ($btn_flag === true) {
        $html .= sprintf($btn_base, $order_common_seqno #1
            , $order_common_seqno #2
            //, $order_common_seqno #3
            , $order_common_seqno #4
            , $order_state        #4
            , $order_common_seqno #5
            , $order_state        #5
            , $order_common_seqno); #6
    }

    $html .= "\n     </div>";
    $html .= "\n </td>";

    //$html  = sprintf($html, number_format($param["event_price"]));

    return $html;
}

//관심상품리스트
function makePrdtListHtml($conn, $rs, $param) {
    $ret = "";

    $upper_tr  = "<tr>";
    $upper_tr .=     "<td><label class=\"check\"><input type=\"checkbox\" name=\"seq\" class=\"_individual\" value=\"%s\"><i><i></i></i></label></td>"; //#0 seqno
    $upper_tr .=     "<td>%s</td>"; //#1 등록일
    $upper_tr .=     "<td id=\"title_td_%s\" class=\"left\">";  // #2 td id
    $upper_tr .=         "<div class=\"subject\">";
    $upper_tr .=             "<span class=\"category_text\">[%s]</span>&nbsp;<span class=\"order_list_title_text\">%s</span><br>"; // #3 카테고리, 제목
    $upper_tr .=             "<ul class=\"information\">";
    $upper_tr .=                 "%s"; // #4 세부정보
    $upper_tr .=             "</ul>";
    $upper_tr .=         "</div>";
    $upper_tr .=     "</td>";
    $upper_tr .=     "<td class=\"left\">";
    $upper_tr .=         "<ul class=\"post\">";
    $upper_tr .=            "%s";   // #F2 5 후가공
    $upper_tr .=         "</ul>";
    $upper_tr .=     "</td>";
    $upper_tr .=     "<td class=\"amount\">";
    $upper_tr .=        "%s%s(%s)";   // #6 수량
    $upper_tr .=     "</td>";
    $upper_tr .=     "<td class=\"btn\">";
    $upper_tr .=         "<button type=\"button\" class=\"goOrder\" onclick=\"goProduct('%s');\" title=\"주문하기\">주문하기</button>"; //#7 seqno
    $upper_tr .=     "</td>";
    $upper_tr .= "</tr>";

    $i = 1;
    while ($rs && !$rs->EOF) {
        $fields = $rs->fields;

        $ret .= "<tbody class=\"multi\" name=\"prdt_list\">";

        if (!empty($fields["amt"]))  {
            $amt = doubleval($fields["amt"]);

            if ($amt < 1) {
                $amt = number_format($amt, 1);
            } else {
                $amt = number_format($amt);
            }
        } else {
            $amt = "";
        }


        $ret .= sprintf($upper_tr, $fields["interest_prdt_seqno"]    //#0
            , explode(' ', $fields["regi_date"])[0] //#1
            , $i                                //#2
            , trim(explode(',', (explode('/', $fields["order_detail"])[0]))[0])           //#3
            , $fields["title"]                  //#3
            , explode('/', $fields["order_detail"], 2)[1] //#4
            , makeAfterHtml($conn, $fields["interest_prdt_seqno"], "interest")                        //#F2 5 후가공
            , $amt                              //#6 수량
            , $fields["amt_unit_dvs"]           //#6 수량
            , $fields["count"]                  //#6 수량
            , $fields["interest_prdt_seqno"]    //#7 seqno
        );

        $ret .= sprintf($lower_tr, $i++);

        $ret .= "</tbody>";

        $rs->moveNext();
    }

    return $ret;
}

// 후공정 옵션 생성 #F
function makeAfterHtml($conn, $seq, $flag) {
    $param = [];
    $util = new FrontCommonUtil();

    if ($flag == "interest") {
        $dao = new InterestDAO();
        $param["interest_prdt_seqno"] = $seq;
        $aft_rs = $dao->selectInterestAfterSet($conn, $param);

    } else {
        $dao = new OrderInfoDAO();
        $param["order_seqno"] = $seq;
        $aft_rs = $dao->selectOrderAfterSet($conn, $param);
    }

    $after_html = '';
    while ($aft_rs && !$aft_rs->EOF) {
        $fields = $aft_rs->fields;

        if ($fields["basic_yn"] === 'Y') {
            $aft_rs->MoveNext();
            continue;
        }

        $aft = sprintf("%s(%s)", $fields["after_name"]
            , $util->getOptAfterFullName($fields));

        $after_html .= $aft . " / ";

        $aft_rs->MoveNext();
    }

    $after_html = substr($after_html, 0, -2);

    return $after_html;
}

// 후공정 옵션 생성 #F
function makeOptHtml($conn, $seq, $flag) {
    $param = [];
    $util = new FrontCommonUtil();

    if ($flag == "interest") {
        $dao = new InterestDAO();
        $param["interest_prdt_seqno"] = $seq;
        $aft_rs = $dao->selectInterestAfterSet($conn, $param);

    } else {
        $dao = new OrderInfoDAO();
        $param["order_seqno"] = $seq;
        $aft_rs = $dao->selectOrderOptSet($conn, $param);
    }

    $opt_html = '';
    while ($aft_rs && !$aft_rs->EOF) {
        $fields = $aft_rs->fields;

        if ($fields["basic_yn"] === 'Y') {
            $aft_rs->MoveNext();
            continue;
        }

        $aft = sprintf("%s(%s)", $fields["opt_name"]
            , $util->getOptAfterFullName($fields));

        $opt_html .= $aft . " / ";

        $aft_rs->MoveNext();
    }

    $opt_html = substr($opt_html, 0, -2);

    return $opt_html;
}
?>
