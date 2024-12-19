<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/common_lib/CommonUtil.inc");

/**
 * @brief 주문 리스트 html 생성
 *
 * @param $conn = connection identifier
 * @param $dao  = 후공정, 옵션 가격 검색을 위한 dao
 * @param $rs   = 주문 리스트 검색결과
 *
 * @return 결과 배열(마지막 seqno랑 주문리스트 html 반환)
 */
function makeOrderListHtml($conn, $dao, $rs, $selected_seqno) {
    $html_base  = "<tr class=\"add_tr %s\" id=\"add_tr_%s\" %s>";
    $html_base .= "  <td><input type='checkbox' id='%s' name='chk_list'/></td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td onclick=\"clickOrderDetail('%s');\">%s</td>";
    $html_base .= "  <td onclick=\"clickOrderDetail('%s');\">%s</td>"; // 주문상세
    $html_base .= "  <td>%s</td>"; // 사이즈
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s건</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s원</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "</tr>";

    $html_base .= "<tr class=\"add_tr_view\" id=\"add_tr_view_%s\">";
    $html_base .= "    <td colspan=\"15\" class=\"hover_none\">";
    $html_base .= "        <table class=\"table_basic_layout_datail\">";
    $html_base .= "            <colgroup>";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "            </colgroup>";
    $html_base .= "            <caption>";
    $html_base .= "                <span class=\"tableTitle\">주문정보&nbsp;-&nbsp;<strong class=\"text_accent_01\">[ 주문번호 : %s / 후가공 : %s ]</strong></span>";
    $html_base .= "                <span class=\"btn_Topmenu_wrapper\">";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"showHistory('%s')\">주문이력보기</button>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"printDetail('%s')\">상세보기인쇄</button>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"attachDlvr('%s')\">택배첨부서류</button>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"printDlvr('%s')\">택배 청구서인쇄</button>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"sendMsg('%s')\">문자발송</button>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"change_state_toerror('%s')\">에러처리</button>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"excelDlvr('%s')\">엑셀송장</button>";
    $html_base .= "                </span>";
    $html_base .= "            </caption>";
    $html_base .= "            <tbody>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>품목</th>";
    $html_base .= "                    <td>%s</td>";
    $html_base .= "                    <th>사이즈</th>";
    $html_base .= "                    <td>%s</td>";
    $html_base .= "                    <th>제목</th>";
    $html_base .= "                    <td colspan=\"3\">%s</td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>재질</th>";
    $html_base .= "                    <td>%s</td>";
    $html_base .= "                    <th>수량</th>";
    $html_base .= "                    <td>%s / %s건</td>";
    $html_base .= "                    <th>첨부파일</th>";
    $html_base .= "                    <td colspan=\"3\"><a href=\"%s\" download><span class=\"downLink\">%s</span> <span>[%s / %s]</span></a></td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>인쇄<br>도수</th>";
    $html_base .= "                    <td>%s</td>";
    $html_base .= "                    <th>후가공<br>내역</th>";
    $html_base .= "                    <td style=\"white-space:pre-wrap; word-wrap:break-word\"><strong>%s</strong></td>";
    $html_base .= "                    <th>고객<br>요청사항</th>";
    $html_base .= "                    <td colspan=\"3\"><span>%s</span></td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>접수사이즈</th>";
    $html_base .= "                    <td><input type='text' id='receipt_size_wid_%s' value='%s' style='width: 50px'> x <input type='text' id='receipt_size_vert_%s' value='%s' style='width: 50px'><input type='button' class=\"btn_memo\" onclick=\"changeReceiptSize('%s')\" value='변경'></td>";
    $html_base .= "                    <th>작업메모</th>";
    $html_base .= "                    <td colspan=\"5\">";
    $html_base .= "                        <textarea id=\"work_memo_%s\" name=\"work_memo\" class=\"memo_textarea\">%s</textarea>";
    $html_base .= "                        <div class=\"btn_memo_wrapper\">";
    $html_base .= "                            <button type=\"button\" id=\"btn_memo\" class=\"btn_memo\" onclick=\"saveMemo('%s')\">메모저장</button>";
    $html_base .= "                        </div>";
    $html_base .= "                    </td>";
    $html_base .= "                </tr>";
    $html_base .= "            </tbody>";
    $html_base .= "         </table>";
    $html_base .= "         <!-- 주문정보 END-->";

    $html_base .= "        <!-- 결제정보 START-->";
    $html_base .= "        <table class=\"table_basic_layout_datail\">";
    $html_base .= "            <colgroup>";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "            </colgroup>";
    $html_base .= "            <caption>";
    $html_base .= "                <span class=\"tableTitle\">결제정보&nbsp;-&nbsp;<strong class=\"text_accent_01\">[ 선입금잔액 : %s / 결제방법 : %s ]</strong></span>";
    $html_base .= "                <span class=\"btn_Topmenu_wrapper\">";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"updatePrice('%s')\">금액 수정</button>";
    $html_base .= "                </span>";
    $html_base .= "            </caption>";
    $html_base .= "            <tbody>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>주문금액</th>";
    $html_base .= "                    <td>";
    $html_base .= "                        <div>";
    $html_base .= "                            <span>공급가액 : </span>";
    $html_base .= "                            <span>%s원</span>";
    $html_base .= "                        </div>";
    $html_base .= "                        <div>";
    $html_base .= "                            <span>부가세액 : </span>";
    $html_base .= "                            <span>%s원</span>";
    $html_base .= "                        </div>";
    $html_base .= "                        <div>";
    $html_base .= "                            <span>선불배송 : </span>";
    $html_base .= "                            <span>%s원</span>";
    $html_base .= "                        </div>";
    $html_base .= "                        <div>";
    $html_base .= "                            <span>합계금액 : </span>";
    $html_base .= "                            <span>%s원</span>";
    $html_base .= "                        </div>";
    $html_base .= "                    </td>";
    $html_base .= "                    <th>";
    $html_base .= "                        <div>접수담당</div>";
    $html_base .= "                        <div>출고담당</div>";
    $html_base .= "                        <div>경리담당</div>";
    $html_base .= "                        <div>배송담당</div>";
    $html_base .= "                    <td>";
    $html_base .= "                        <div>%s</div>";
    $html_base .= "                        <div>T. 02-2260-9029</div>";
    $html_base .= "                        <div>T. 02-2260-9047</div>";
    $html_base .= "                        <div>T. 02-2260-9029</div>";
    $html_base .= "                    </td>";
    $html_base .= "                    <th>금액<br>수정내역</th>";
    $html_base .= "                    <td>";
    $html_base .= "                        <div>요청 : 없음</div>";
    $html_base .= "                        <div>항목 : </div>";
    $html_base .= "                        <div>수정사유 : </div>";
    $html_base .= "                        <div>일시 : </div>";
    $html_base .= "                    </td>";
    $html_base .= "                    <th>주문수정<br>내역</th>";
    $html_base .= "                    <td>없음</td>";
    $html_base .= "                </tr>";
    $html_base .= "            </tbody>";
    $html_base .= "        </table>";
    $html_base .= "        <!-- 결제정보 END-->";

    $html_base .= "        <!-- 접수정보 START-->";
    $html_base .= "        <table class=\"table_basic_layout_datail\">";
    $html_base .= "            <colgroup>";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "            </colgroup>";
    $html_base .= "            <caption>";
    $html_base .= "                <span class=\"tableTitle\">접수정보&nbsp;-&nbsp;<strong class=\"text_accent_01\">[ 접수담당 : %s ]</strong></span>";
    $html_base .= "                <span class=\"btn_Topmenu_wrapper\">";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"uploadAfter('%s')\">후가공 업로드</button>";
    $html_base .= "                </span>";
    $html_base .= "            </caption>";
    $html_base .= "            <tbody>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>미리보기</th>";
    $html_base .= "                    <td colspan=\"7\">";
    $html_base .= "                        <div style=\"display: inline-flex;\">%s</div>";
    $html_base .= "                    </td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>접수파일</th>";
    $html_base .= "                    <td colspan=\"7\">";
    $html_base .= "                        <div><a href=\"%s\" download><span class=\"downLink\">%s</span></a></div>";
    $html_base .= "                    </td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>후가공 파일</th>";
    $html_base .= "                    <td colspan=\"7\">";
    $html_base .= "                        <div><a href=\"\" download><span class=\"downLink\"></span></a></div>";
    $html_base .= "                    </td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>접수메모</th>";
    $html_base .= "                    <td colspan=\"7\">";
    $html_base .= "                        <textarea id=\"accept_memo_%s\" name=\"accept_memo\" class=\"memo_textarea\">%s</textarea>";
    $html_base .= "                        <div class=\"btn_memo_wrapper\">";
    $html_base .= "                            <button type=\"button\" id=\"btn_memo\" class=\"btn_memo\" onclick=\"saveAcceptMemo('%s')\">메모저장</button>";
    $html_base .= "                        </div>";
    $html_base .= "                    </td>";
    $html_base .= "                </tr>";
    $html_base .= "            </tbody>";
    $html_base .= "        </table>";
    $html_base .= "        <!-- 접수정보 END-->";

    $html_base .= "        <!-- 회원/배송정보 START-->";
    $html_base .= "        <table class=\"table_basic_layout_datail\">";
    $html_base .= "            <colgroup>";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "            </colgroup>";
    $html_base .= "            <caption>";
    $html_base .= "                <span class=\"tableTitle\">회원/배송정보&nbsp;-&nbsp;<strong class=\"text_accent_01\">[ 배송담당 : %s / 전산덩어리 : 1개 / 실측덩어리 : 2개 / 전산무게 : %skg / 실측무게 : %skg ]</strong></span>";
    $html_base .= "                <span class=\"btn_Topmenu_wrapper\">";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"setWeight('%s')\">무게/덩어리</button>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"setOutsourcing('%s')\">외주입력</button>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"updateInvoice('%s')\">송장번호 입력/수정</button>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"updateAddr('%s')\">주소/수신정보 수정</button>";
    $html_base .= "                </span>";
    $html_base .= "            </caption>";
    $html_base .= "            <tbody>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>회원정보</th>";
    $html_base .= "                    <td>";
    $html_base .= "                        <div>";
    $html_base .= "                            <span>아이디 : </span>";
    $html_base .= "                            <span>%s</span>";
    $html_base .= "                        </div>";
    $html_base .= "                        <div>";
    $html_base .= "                            <span>회원명 : </span>";
    $html_base .= "                            <span>%s</span>";
    $html_base .= "                        </div>";
    $html_base .= "                        <div>";
    $html_base .= "                            <span>연락처 : </span>";
    $html_base .= "                            <span>%s</span>";
    $html_base .= "                        </div>";
    $html_base .= "                        <div>";
    $html_base .= "                            <span>휴대폰 : </span>";
    $html_base .= "                            <span>%s</span>";
    $html_base .= "                        </div>";
    $html_base .= "                    </td>";
    $html_base .= "                    <th>";
    $html_base .= "                        <div>발신인</div>";
    $html_base .= "                        <div>배송방법</div>";
    $html_base .= "                    <td>";
    $html_base .= "                        <div>%s</div>";
    $html_base .= "                        <div>%s</div>";
    $html_base .= "                    </td>";
    $html_base .= "                    <th>수신정보</th>";
    $html_base .= "                    <td>";
    $html_base .= "                        <div>수령인 : <span>%s</span></div>";
    $html_base .= "                        <div>연락처 : <span>%s</span></div>";
    $html_base .= "                        <div>휴대폰 : <span>%s</span></div>";
    $html_base .= "                    </td>";
    $html_base .= "                    <th>배송주소</th>";
    $html_base .= "                    <td>";
    $html_base .= "                        <div>%s</div>";
    $html_base .= "                        <div>%s</div>";
    $html_base .= "                        <div>%s</div>";
    $html_base .= "                    </td>";
    $html_base .= "                </tr>";
    $html_base .= "            </tbody>";
    $html_base .= "        </table>";
    $html_base .= "        <!-- 회원/배송정보 END-->";
    $html_base .= "        <!-- 작업진행현황 START-->";
    $html_base .= "        <table class=\"table_basic_layout_datail\">";
    $html_base .= "            <colgroup>";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "            </colgroup>";
    $html_base .= "            <caption>";
    $html_base .= "                <span class=\"tableTitle\">작업진행현황&nbsp;-&nbsp;<strong class=\"text_accent_01\">[ 판번호 : <a href=\"%s\" download><span class=\"downLink\">%s</span></a> , 딱판 : <a href=\"%s\" download><span class=\"downLink\">%s</span></a> ]</strong></span>";
    $html_base .= "            </caption>";
    $html_base .= "            <tbody>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>작업정보</th>";
    $html_base .= "                    <td colspan=\"7\">";
    $html_base .= "                        <div>";
    $html_base .= "                            %s";
    $html_base .= "                        </div>";
    $html_base .= "                    </td>";
    $html_base .= "                </tr>";
    $html_base .= "            </tbody>";
    $html_base .= "        </table>";
    $html_base .= "        <!-- 작업진행현황 END-->";
    $html_base .= "        <div class=\"btn_bottom_wrapper\">";
    $html_base .= "            <button type=\"button\" class=\"btn_bottom\" onclick=\"updateOrder('%s','%s')\">주문수정</button>";
    $html_base .= "            <button type=\"button\" class=\"btn_bottom\" onclick=\"reworkOrder('%s','%s','%s','%s')\">재작업/주문</button>";
    $html_base .= "            <button type=\"button\" class=\"btn_bottom\" onclick=\"goUserPage('%s')\">사용자화면</button>";
    $html_base .= "            <button type=\"button\" class=\"btn_bottom\" onclick=\"deleteOrder('%s')\">주문삭제</button>";
    $html_base .= "            <button type=\"button\" class=\"btn_bottom\" onclick=\"hideOrderDetail('%s')\">닫기</button>";
    $html_base .= "        </div>";

    $html_base .= "    </td>";
    $html_base .= "</tr>";

    $util = new CommonUtil();

    $html = "";
    $first_seq = "";
    $last_seq = "";
    $i = 0;
    while ($rs && !$rs->EOF) {
        $seqno  = $rs->fields["seqno"];
        /*
        $state_arr = $_SESSION["state_arr"];
        $disabled = "disabled=\"disabled\"";
        $disabled2 = "disabled=\"disabled\"";
        $state = $util->statusCode2status($rs->fields["order_state"]);

        if ($rs->fields["order_state"] > $state_arr["접수대기"]) {
            $disabled = "";
        } else if ($rs->fields["order_state"] > $state_arr["조판대기"]) {
            $disabled = "";
            $disabled2 = "";
        }
        */
        
        $tr_class = (($i++ % 2) === 0) ? 'cellbg' : '';

        $order_detail_arr = explode(' / ', $rs->fields["order_detail"]);
        $temp = array();
        $temp["order_common_seqno"] = $seqno;
        $order_after_arr = $dao->selectOrderAfterInfo($conn, $temp);
        $after_detail = $order_after_arr->fields["after_detail"];
        $order_mng = "<span style=\"color:red; font-weight: bold;\">접수전</span>";
        if ($rs->fields["order_mng"]) {
            $order_mng = $rs->fields["order_mng"];
        }

        $size = $rs->fields["size"];
        $file_size = "";
        if($size < 1024){
            $file_size = number_format($size * 1.024).'b';
        }else if(($size > 1024) && ($size < 1024000)){
            $file_size = number_format($size * 0.001024).'Kb';
        }else if($size > 1024000){
            $file_size = number_format($size * 0.000001024,2).'Mb';
        }
        
            $upload_time = date("Y-m-d", strtotime($rs->fields["order_regi_date"]));

        if ($rs->fields["upload_time"]) {
            $upload_time = $rs->fields["upload_time"];
        }

        if($selected_seqno == "'".$seqno."'") {
            $temp = array();
            $temp["col"] = "*";
            $temp["table"] = "order_dlvr";
            $temp["where"]["order_common_seqno"] = $rs->fields["order_common_seqno"];
            $temp["where"]["tsrs_dvs"] = "송신";

            $order_dlvr_ts = $dao->selectData($conn, $temp);

            $temp = array();
            $temp["col"] = "*";
            $temp["table"] = "order_dlvr";
            $temp["where"]["order_common_seqno"] = $rs->fields["order_common_seqno"];
            $temp["where"]["tsrs_dvs"] = "수신";

            $order_dlvr_rs = $dao->selectData($conn, $temp);

            $order_dlvr_info = "배송전";
            if ($order_dlvr_ts->fields["name"]) {
                $order_dlvr_info = $order_dlvr_ts->fields["name"];
            }
        }
        $member_name_info = $rs->fields["member_name"];
        if ($rs->fields["office_nick"]) {
            $member_name_info .= " <span style=\"color:blue; font-weight: bold;\">[" . $rs->fields["office_nick"] . "]</span>";
        }
        $tr_color = "";
        if($rs->fields["erp_state_name"] == "주문취소") {
            $tr_color = "style='background: lightpink !important'";
        }

        if($rs->fields["erp_state_name"] == "파일에러") {
            $tr_color = "style='background: gold !important'";
        }
        $preview_img = "";
        $j = 0;
        if ($rs->fields["preview_file_name"]) {
            foreach (explode('||', $rs->fields["preview_file_name"]) as $value) {
                if ($j > 0) {
                    $preview_img .= "<br>&nbsp;&nbsp;&nbsp;";
                }
                $preview_img .= "<img src=\"" . $rs->fields["preview_file_path"] . $value . "\"style=\"max-width:300px;max-height:300px;\">";
                $j++;
            }
        }

        $title = $rs->fields["title"];

        $temp = array();
        $temp["col"]   = "rework_cause";
        $temp["table"] = "rework_list";
        $temp["where"]["rework_order_common_seqno"] = $seqno;
        $rework_rs = $dao->selectData($conn, $temp);
        $order_detail = $rs->fields["order_detail"];
        if($rework_rs->fields["rework_cause"] != null) {
            $order_detail = "<span style='color:red; font-weight: bold;'>[" . $rework_rs->fields["rework_cause"] . "] </span>" . $order_detail;
        }

        if($selected_seqno == "'".$seqno."'") {
            $temp = array();
            $temp["order_common_seqno"] = $seqno;
            $order_history_arr = $dao->selectOrderHistoryInfo($conn, $temp);
            $order_history = "";
            $inserted_state = array();
            while ($order_history_arr && !$order_history_arr->EOF) {
                $state = $order_history_arr->fields["front_state_name"];
                if($state == "후가공완료") {
                    $state = $order_history_arr->fields["detail"] . "완료";
                }
                if($inserted_state[$state] == 1) {
                    $order_history_arr->MoveNext();
                    continue;
                }
                $inserted_state[$state] = 1;
                if($state != "완료")
                    $order_history = "<span>" . $state . " : " . $order_history_arr->fields["state_modi_date"] . " " . $order_history_arr->fields["name"] . "</span><br><br>" . $order_history;
                $order_history_arr->MoveNext();
            }
            $typset_history_arr = $dao->selectSheetTypsetInfo($conn, $temp);
        }


        $html .= sprintf($html_base, $tr_class
                                    , $seqno
                                    , $tr_color
                                   , $rs->fields["order_num"]
                                   , $rs->fields["order_num"]
                                   , date("Y-m-d", strtotime($rs->fields["order_regi_date"]))
                                   , $order_mng
                                   , $member_name_info
                                   , $seqno
                                   , $title
                                   , $seqno
                                   , $order_detail
                                   , $rs->fields["cut_size_wid"] . "x" . $rs->fields["cut_size_vert"]
                                   , $after_detail
                                   , $rs->fields["erp_state_name"]
                                   , number_format($rs->fields["count"])
                                   , number_format($rs->fields["amt"]) . $rs->fields["amt_unit_dvs"]
                                   , number_format($rs->fields["sell_price"])
                                   , $rs->fields["cust_memo"]
                                   , $rs->fields["dlvr_dvs"]
                                   , $seqno
                                   , $rs->fields["order_num"]
                                   , $order_detail_arr[4]
                                   , $seqno
                                   , $seqno
                                   , $seqno
                                   , $seqno
                                   , $seqno
                                   , $rs->fields["order_num"]
                                   , $seqno
                                   , $order_detail_arr[0]
                                   , $rs->fields["cut_size_wid"] . "x" . $rs->fields["cut_size_vert"] . " (" . $rs->fields["work_size_wid"] . "x" . $rs->fields["work_size_vert"] . ")"
                                   , $rs->fields["title"]
                                   , $order_detail_arr[1]
                                   , number_format($rs->fields["amt"]) . $rs->fields["amt_unit_dvs"]
                                   , number_format($rs->fields["count"])
                                   , $rs->fields["file_path"] . $rs->fields["save_file_name"]
                                   , $rs->fields["origin_file_name"]
                                   , $file_size
                                   , $upload_time
                                   , $order_detail_arr[3]
                                   , $after_detail
                                   , $rs->fields["cust_memo"]
                                   , $seqno
                                   , $rs->fields["receipt_size_wid"]
                                   , $seqno
                                   , $rs->fields["receipt_size_vert"]
                                   , $seqno
                                   , $seqno
                                   , $rs->fields["work_memo"]
                                   , $seqno
                                   , 0
                                   , $rs->fields["pay_way"]
                                   , $seqno
                                   , number_format($rs->fields["pay_price"] - $rs->fields["pay_price"]/10)
                                   , number_format($rs->fields["pay_price"]/10)
                                   , number_format($rs->fields["pay_price"])
                                   , number_format($rs->fields["pay_price"])
                                   , $order_mng
                                   , $order_mng
                                   , $seqno
                                   , $preview_img
                                   , $rs->fields["accept_file_path"] . $rs->fields["accept_file_name"]
                                   , $rs->fields["accept_file_name"]
                                   , $seqno
                                   , $rs->fields["accept_memo"]
                                   , $seqno
                                   , $order_dlvr_info
                                   , $rs->fields["expec_weight"]
                                   , $rs->fields["expec_weight"]
                                   , $seqno
                                   , $seqno
                                   , $seqno
                                   , $seqno
                                   , $rs->fields["mail"]
                                   , $rs->fields["member_name"]
                                   , $rs->fields["tel_num"]
                                   , $rs->fields["cell_num"]
                                   , $order_dlvr_ts->fields["name"]
                                   , $order_dlvr_rs->fields["invo_cpn"]
                                   , $order_dlvr_rs->fields["name"]
                                   , $order_dlvr_rs->fields["tel_num"]
                                   , $order_dlvr_rs->fields["cell_num"]
                                   , $order_dlvr_rs->fields["zipcode"]
                                   , $order_dlvr_rs->fields["addr"]
                                   , $order_dlvr_rs->fields["addr_detail"]
                                    , $typset_history_arr->fields["typset_pdf"]
                                    , $typset_history_arr->fields["typset_num"]
                                    , $typset_history_arr->fields["typset_label"]
                                    , $typset_history_arr->fields["typset_num"]
                                    , $order_history
                                   , $rs->fields["member_seqno"]
                                   , $seqno
                                   , $seqno
                                    ,$rs->fields["page_cnt"]
                                    ,$rs->fields["count"]
                                    ,$rs->fields["pay_price"]
                                   , $rs->fields["member_seqno"]
                                    , $rs->fields["order_num"]
                                   , $seqno
                                   );
        $rs->MoveNext();
    }

    if ($i == 0) {
        return "<tr><td colspan=\"15\">검색된 내용이 없습니다.</td></tr>";
    }
    return $html;
}

/**
 * @brief 후공정 발주 리스트 html 생성
 *
 * @param $conn  = connection identifier
 * @param $dao   = 수주처 검색을 위한 dao
 * @param $rs    = 주문 리스트 검색결과
 * @param $param = 카테고리
 *
 * @return 결과 배열(마지막 seqno랑 주문리스트 html 반환)
 */
function makeAfterListHtml($conn, $dao, $rs, $cate_name) {
    $html_base  = "<tr %s>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td>%s</td>";
    $html_base .= "  <td><button type=\"button\" class=\"green btn_pu btn fix_height20 fix_width40\" onclick=\"getAfterInfo('%s', '%s');\">보기</button></td>";
    $html_base .= "</tr>";

    $html = "";
    $i = 1;
    while ($rs && !$rs->EOF) {

        $tr_class = (($i % 2) === 0) ? "class=\"cellbg\"" : '';

        $param = array();
        $param["table"] = "extnl_brand";
        $param["col"] = "extnl_etprs_seqno";
        $param["where"]["extnl_brand_seqno"] = $rs->fields["extnl_brand_seqno"];

        $sel_rs = $dao->selectData($conn, $param);
  
        $param = array();
        $param["table"] = "extnl_etprs";
        $param["col"] = "manu_name";
        $param["where"]["extnl_etprs_seqno"] = $sel_rs->fields["extnl_etprs_seqno"];

        $sel_rs = $dao->selectData($conn, $param);

        $html .= sprintf($html_base, $tr_class
                                   , $i
                                   , $cate_name
                                   , $rs->fields["after_name"]
                                   , $rs->fields["depth1"]
                                   , $rs->fields["depth2"]
                                   , $rs->fields["depth3"]
                                   , $rs->fields["amt"]
                                   , $rs->fields["amt_unit"]
                                   , $rs->fields["orderer"]
                                   , $sel_rs->fields["manu_name"]
                                   , $rs->fields["after_op_seqno"]
                                   , $cate_name);
        $i++;
        $rs->MoveNext();
    }
    return $html;
}

?>
