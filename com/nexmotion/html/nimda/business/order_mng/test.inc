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
function makeOrderListHtml($conn, $dao, $rs, $param) {
    $html_base  = "<tr class=\"add_tr %s\" id=\"add_tr_%s\" %s>";
    $html_base .= "  <td class=\"hover_td\" rowspan=\"2\" %s><input type='checkbox' id='%s' name='chk_list'/></td>";
    $html_base .= "  <td class=\"hover_td\" rowspan=\"2\"><span>%s</span></td>";
    $html_base .= "  <td class=\"hover_td\" rowspan=\"2\"><span>%s <br /> %s <br /> %s</span></td>";
    $html_base .= "  <td class=\"hover_td\"><span>%s</span></td>";
    $html_base .= "  <td class=\"hover_td\"><span>%s</span></td>";
    $html_base .= "  <td class=\"hover_td\" onclick=\"clickOrderDetail('%s');\"><span>%s</span></td>";
    $html_base .= "  <td class=\"hover_td\" onclick=\"clickOrderDetail('%s');\"><span>%s</span></td>"; // 주문상세
    $html_base .= "  <td class=\"hover_td\"><span>%s</span></td>"; // 사이즈
    $html_base .= "  <td class=\"hover_td\"><span>%s</span></td>";
    $html_base .= "  <td class=\"hover_td\"><span>%s</span></td>";
    $html_base .= "  <td class=\"hover_td\"><span>%s</span></td>";
    $html_base .= "  <td class=\"hover_td\"><span>%s</span></td>";
    $html_base .= "  <td class=\"hover_td\"><span>%s원</span></td>";
    $html_base .= "  <td class=\"hover_td\"><span>%s</span></td>";
    $html_base .= "  <td class=\"hover_td\"><span>%s</span></td>";
    $html_base .= "  <td class=\"hover_td\"><span>%s</span></td>";
    $html_base .= "</tr>";
    $html_base .= "<tr class=\"add_tr %s\" id=\"add_tr2_%s\" %s>";
    $html_base .= "  <td colspan=\"2\">";
    $html_base .= "      %s";
    $html_base .= "  </td>";
    $html_base .= "  <td colspan=\"11\">";
    $html_base .= "      %s";
    $html_base .= "  </td>";
    $html_base .= "</tr>";

    $html_base .= "<tr class=\"add_tr_view\" id=\"add_tr_view_%s\" >";
    $html_base .= "    <td colspan=\"16\" class=\"hover_none\">";
    $html_base .= "    <div style=\"display: inline-flex;\">";
    $html_base .= "        <table class=\"table_basic_layout_datail\" style='margin:15px;'>";
    $html_base .= "            <colgroup>";
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
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" style='display: none;' onclick=\"excelDlvr('%s')\">엑셀송장</button>";
    $html_base .= "                </span>";
    $html_base .= "            </caption>";
    $html_base .= "            <tbody>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>품목</th>";
    $html_base .= "                    <td>%s</td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>사이즈</th>";
    $html_base .= "                    <td>%s</td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>제목</th>";
    $html_base .= "                    <td>%s</td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>재질</th>";
    $html_base .= "                    <td>%s</td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>수량</th>";
    $html_base .= "                    <td>%s / %s</td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>첨부파일</th>";
    $html_base .= "                    <td><a href=\"%s\" download><span class=\"downLink\">%s</span> <span>[%s / %s]</span></a></td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>인쇄<br>도수</th>";
    $html_base .= "                    <td>%s</td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>후가공<br>내역</th>";
    $html_base .= "                    <td style=\"white-space:pre-wrap; word-wrap:break-word\"><strong>%s</strong></td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>옵션<br>내역</th>";
    $html_base .= "                    <td style=\"white-space:pre-wrap; word-wrap:break-word\"><strong>%s</strong></td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>고객<br>요청사항</th>";
    $html_base .= "                    <td><span>%s</span></td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>편집사이즈</th>";
    $html_base .= "                    <td><input type='text' id='receipt_size_wid_%s' value='%s' style='width: 50px'> x <input type='text' id='receipt_size_vert_%s' value='%s' style='width: 50px'><input type='button' class=\"btn_memo\" onclick=\"changeReceiptSize('%s')\" style=\"margin-left: 0.5rem;\" value='변경'></td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>작업메모</th>";
    $html_base .= "                    <td>";
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
    $html_base .= "        <table class=\"table_basic_layout_datail\" style='margin:15px;'>";
    $html_base .= "            <colgroup>";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "            </colgroup>";
    $html_base .= "            <caption>";
    $html_base .= "                <span class=\"tableTitle\">결제정보&nbsp;-&nbsp;<strong class=\"text_accent_01\">[ 선입금잔액 : %s / 결제방법 : %s ]</strong></span>";
    $html_base .= "                <span class=\"btn_Topmenu_wrapper\" style='margin-top:20px;'>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"showChangePrice('%s')\">금액 수정</button>";
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
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
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
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>금액<br>수정내역</th>";
    $html_base .= "                    <td>";
    $html_base .= "                        <div>요청 : 없음</div>";
    $html_base .= "                        <div>항목 : </div>";
    $html_base .= "                        <div>수정사유 : </div>";
    $html_base .= "                        <div>일시 : </div>";
    $html_base .= "                    </td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>주문수정<br>내역</th>";
    $html_base .= "                    <td>없음</td>";
    $html_base .= "                </tr>";
    $html_base .= "            </tbody>";
    $html_base .= "        </table>";
    $html_base .= "        <!-- 결제정보 END-->";
    $html_base .= "        <!-- 회원/배송정보 START-->";
    $html_base .= "        <table class=\"table_basic_layout_datail\" style='margin:15px;'>";
    $html_base .= "            <colgroup>";
    $html_base .= "                <col width=\"100px;\">";
    $html_base .= "                <col width=\"\">";
    $html_base .= "            </colgroup>";
    $html_base .= "            <caption>";
    $html_base .= "                <span class=\"tableTitle\">회원/배송정보&nbsp;-&nbsp;<strong class=\"text_accent_01\" style='display: none'>[ 배송담당 : %s / 전산덩어리 : 1개 / 실측덩어리 : 2개 / 전산무게 : %skg / 실측무게 : %skg ]</strong></span>";
    $html_base .= "                <span class=\"btn_Topmenu_wrapper\" style='margin-top:20px;'>";
    $html_base .= "                    <button type=\"button\" value=\"\" class=\"btn_Topmenu\" onclick=\"openBunDlvr('%s')\">묶음보기</button>";
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
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>";
    $html_base .= "                        <div>발신인</div>";
    $html_base .= "                        <div>배송방법</div>";
    $html_base .= "                    <td>";
    $html_base .= "                        <div>%s</div>";
    $html_base .= "                        <div>%s</div>";
    $html_base .= "                        <div>%s</div>";
    $html_base .= "                    </td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
    $html_base .= "                    <th>수신정보</th>";
    $html_base .= "                    <td>";
    $html_base .= "                        <div>수령인 : <span>%s</span></div>";
    $html_base .= "                        <div>연락처 : <span>%s</span></div>";
    $html_base .= "                        <div>휴대폰 : <span>%s</span></div>";
    $html_base .= "                    </td>";
    $html_base .= "                </tr>";
    $html_base .= "                <tr>";
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
    $html_base .= "    </div>";
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
    $html_base .= "                        <div style=\"width: 100%%;display: inline-flex;overflow-x: scroll;overflow-y: hidden;white-space: nowrap;\">%s</div>";
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
    $html_base .= "                <span class=\"tableTitle\">작업진행현황&nbsp;-&nbsp;<strong class=\"text_accent_01\">[ 판번호 : %s , 딱판 : %s ]</strong></span>";
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
    $html_base .= "            <button type=\"button\" class=\"btn_bottom %s\" onclick=\"updateOrder('%s','%s')\">주문수정</button>";
    $html_base .= "            <button type=\"button\" class=\"btn_bottom\" onclick=\"reworkOrder('%s','%s','%s','%s')\">재작업/주문</button>";
    $html_base .= "            <button type=\"button\" class=\"btn_bottom\" onclick=\"goUserPage('%s', '%s')\">사용자화면</button>";
    $html_base .= "            <button type=\"button\" class=\"btn_bottom\" onclick=\"deleteOrder('%s')\">주문삭제</button>";
    $html_base .= "            <button type=\"button\" class=\"btn_bottom\" onclick=\"fileError('%s')\">파일에러</button>";
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


        if($rs->fields["OPI_Date"] == null) {
            $chk_color = "style='background:blueviolet;'";
        } else {
            $chk_color = "";
        }
        $order_update_btn = "";
        $state_class1 = "";
        $state_class2 = "";
        $state_class3 = "";
        $state_class4 = "";
        $state_class5 = "";
        $state_class6 = "";
        $state_class7 = "";
        if ($rs->fields["state_code"] > 1380) {
            $state_class1 = "list-btn-on";
            //$state_class2 = "";
            //$state_class3 = "";
            //$state_class4 = "";
            //$state_class5 = "";
            //$state_class6 = "";
        }
        if ($rs->fields["state_code"] >= 1380) {
            //$state_class1 = "";
            $state_class2 = "list-btn-on";
            //$state_class3 = "";
            //$state_class4 = "";
            //$state_class5 = "";
            //$state_class6 = "";
        }
        if ($rs->fields["state_code"] >= 2220) {
            //$state_class1 = "";
            //$state_class2 = "";
            $state_class3 = "list-btn-on";
            //$state_class4 = "";
            //$state_class5 = "";
            //$state_class6 = "";
            $order_update_btn = "order_update_btn-hide";
        }
        if ($rs->fields["state_code"] >= 2320) {
            //$state_class1 = "";
            //$state_class2 = "";
            //$state_class3 = "";
            $state_class4 = "list-btn-on";
            //$state_class5 = "";
            //$state_class6 = "";
            $order_update_btn = "order_update_btn-hide";
        }

        if ($rs->fields["state_code"] >= 2420) {
            //$state_class1 = "";
            //$state_class2 = "";
            //$state_class3 = "";
            //$state_class4 = "";
            $state_class5 = "list-btn-on";
            //$state_class6 = "";
            $order_update_btn = "order_update_btn-hide";
        }
        if ($rs->fields["state_code"] >= 2480 && $rs->fields["state_code"] != 2620) {
            //$state_class1 = "";
            //$state_class2 = "";
            //$state_class3 = "";
            //$state_class4 = "";
            //$state_class5 = "";
            $state_class6 = "list-btn-on";
            $order_update_btn = "order_update_btn-hide";
        }
        if ($rs->fields["state_code"] >= 2780) {
            //$state_class1 = "";
            //$state_class2 = "";
            //$state_class3 = "";
            //$state_class4 = "";
            //$state_class5 = "";
            $state_class8 = "list-btn-on";
        }
        if ($rs->fields["state_code"] >= 3220) {
            //$state_class1 = "";
            //$state_class2 = "";
            //$state_class3 = "";
            //$state_class4 = "";
            //$state_class5 = "";
            $state_class7 = "list-btn-on";
            $order_update_btn = "order_update_btn-hide";
        }

        $list_order_history = "";
        //$list_order_history .= "\n<button type=\"button\" class=\"list-btn-state " . $state_class1 . "\">접수대기</button>";
        $list_order_history .= "\n<button type=\"button\" class=\"list-btn-state " . $state_class2 . "\">접수완료</button>";
        $list_order_history .= "\n<button type=\"button\" class=\"list-btn-state " . $state_class3 . "\">조판완료</button>";
        $list_order_history .= "\n<button type=\"button\" class=\"list-btn-state " . $state_class4 . "\">출력완료</button>";
        $list_order_history .= "\n<button type=\"button\" class=\"list-btn-state " . $state_class5 . "\">인쇄완료</button>";

        $tr_class = (($i++ % 2) === 0) ? 'cellbg' : '';

        $order_detail_arr = explode(' / ', $rs->fields["order_detail"]);
        $temp = array();
        $temp["order_common_seqno"] = $seqno;
        $temp["sortcode"] = $param['sortcode'];

        $order_after_arr = $dao->selectOrderAfterInfo($conn, $temp);
        //$order_opt_arr = $dao->selectOrderOptInfo($conn, $temp);
        
        $aft_name = trim(explode(' / ',$order_after_arr->fields["after_detail"])[0]," ");

        $isBasic = trim(explode(' / ',$order_after_arr->fields["after_detail"])[2]);
        $after_detail = "";
        /*
        if($aft_name != "재단" &&  $aft_name != "도무송" && $aft_name != "봉투가공" && $aft_name != "코팅") {
            $after_detail = $order_after_arr->fields["after_detail"];
        }
        */
        //echo $order_after_arr->fields["after_name"];
        //$order_after_arr->fields["after_name"] = explode('-', $order_after_arr->fields["after_name"])[0] ."(". explode("-", $aft_name)[1] . ")";
        $after_name_arr = explode('|', $order_after_arr->fields["after_name"]);
        $is_included_cutting = false;
        $is_envelope = strpos($rs->fields["order_num"], "EV") !== false ? true : false;
        if($is_envelope) {
            $list_order_history .= "\n<button type=\"button\" class=\"list-btn-state " . $state_class8 . "\">봉투가공</button>";
        }
        $after_i = 0;
        if(startsWith($rs->fields["cate_sortcode"], "004")) {
            if(strpos($rs->fields["order_detail"], "원터치")) {
                $after_detail .= "원터치";
                $after_i++;
            }

            if(strpos($rs->fields["order_detail"], "투터치")) {
                $after_detail .= "투터치";
                $after_i++;
            }

            if(strpos($rs->fields["order_detail"], "도무송")) {
                $after_detail .= "도무송 - " . explode(" / ", $rs->fields["order_detail"])[2];
                $after_i++;
            }
        }

        $str_count = number_format($rs->fields["count"]) . "건";
        if($rs->fields["cate_sortcode"] == "008001001") {
            $str_count = "1건(" . number_format($rs->fields["count"]) . "종)";
        }

        if($rs->fields["cate_sortcode"] == "008001003") {
            $after_detail .= explode(" / ", $rs->fields["order_detail"])[2];
        }
        
        
        foreach ($order_opt_arr as $opt_name){
                $opt_detail = $opt_name[0];
                 
        }
          

  
        foreach($after_name_arr as $after_name) {
            //echo $after_name;
            if($after_name != "") {
                $name = trim(explode('-',$after_name)[0]," ");

                if((($name == "재단" && $rs->fields["OPI_Date"] != null)
                    ||  $name == "도무송"
                    || $name == "봉투가공"
                    || ($name == "코팅" && $rs->fields["cate_sortcode"] != "001002001" && !startsWith($rs->fields["cate_sortcode"], "002") && !startsWith($rs->fields["cate_sortcode"], "011") && $rs->fields["cate_sortcode"] != "001004001" && !startsWith($rs->fields["cate_sortcode"], "005"))))
                    continue;
                /*
                if(strpos($name,"재단") !== false ||
                    strpos($name,"도무송") !== false ||
                    strpos($name,"봉투가공") !== false ||
                    strpos($name,"코팅") !== false) continue;
*/
                $state = trim(explode('-',$after_name)[1]," ");
                $detail = trim(explode('-',$after_name)[3]," ");
                $tmp = trim(explode('-',$after_name)[0]," ");
                if($tmp == "접착" && $is_envelope) $tmp = "양면테입";
                if($after_i > 0) {
                    $after_detail .= ", ";
                }
                $after_detail .= $tmp . " - " . $detail;
                $before_cutting_yn = trim(explode('-',$after_name)[2]," ");

                if(!$is_included_cutting && $before_cutting_yn == "N" && !$is_envelope) {
                    $list_order_history .= "\n<button type=\"button\" class=\"list-btn-state " . $state_class6 . "\">재단완료</button>";
                    $is_included_cutting = true;
                }
                $state_color = "";
                if($state != "") {
                    $state_color = "list-btn-on";
                }
                $list_order_history .= "\n<button type=\"button\" class=\"list-btn-state " . $state_color . "\">" . $name . "완료</button>";
                $after_i++;
            }
        }
        

        if(!$is_included_cutting && !$is_envelope)
            $list_order_history .= "\n<button type=\"button\" class=\"list-btn-state " . $state_class6 . "\">재단완료</button>";

        $dlvr_start = $rs->fields["default_release"];
        $dlvr_way = $rs->fields["dlvr_way"];
        $str_dlvr = "";
        switch ($dlvr_way) {
            case "01" :
                $str_dlvr = "택배발송";
                $dlvr_way = "택배";
                break;
            case "02" :
                $str_dlvr = "출고완료";
                $dlvr_way = "직배";
                $dlvr_start = "성수";
                break;
            case "03" :
                $str_dlvr = "출고완료";
                $dlvr_way = "화물";
                break;
            case "04" :
                $str_dlvr = "출고완료";
                $dlvr_way = "퀵(오토바이)";
                break;
            case "05" :
                $str_dlvr = "출고완료";
                $dlvr_way = "퀵(지하철)";
                break;
            case "06" :
                $str_dlvr = "입고완료(인현)";
                $dlvr_way = "인현동방문";
                $dlvr_start = "인현";
                break;
            case "07" :
                $str_dlvr = "입고완료(성수)";
                $dlvr_way = "성수동방문";
                $dlvr_start = "성수";
                break;
        }

        $list_order_history .= "\n<button type=\"button\" class=\"list-btn-state " . $state_class7 . "\">".$str_dlvr."</button>";
        $order_mng = "<span style=\"color:red; font-weight: bold;\">접수전</span>";
        if ($rs->fields["order_mng"]) {
            $temp = array();
            $temp["empl_id"] = $rs->fields["order_mng"];
            $order_mng = $dao->selectEmplName($conn, $temp)->fields["name"];
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

        if($param['selected_seqno'] == "'".$seqno."'") {
            $temp = array();
            $temp["col"] = "*";
            $temp["table"] = "order_dlvr";
            $temp["where"]["order_common_seqno"] = $seqno;
            $temp["where"]["tsrs_dvs"] = "송신";

            $order_dlvr_ts = $dao->selectData($conn, $temp);

            $temp = array();
            $temp["col"] = "*";
            $temp["table"] = "order_dlvr";
            $temp["where"]["order_common_seqno"] = $seqno;
            $temp["where"]["tsrs_dvs"] = "수신";

            $order_dlvr_rs = $dao->selectData($conn, $temp);

            $order_dlvr_info = "배송전";
            if ($order_dlvr_ts->fields["name"]) {
                $order_dlvr_info = $order_dlvr_ts->fields["name"];
            }


            if(strpos($rs->fields["order_num"],"ES")) {
                $temp = array();
                $temp["order_common_seqno"] = $seqno;

                $paper_arr = $dao->selectEstiPaperInfo($conn, $temp);
                $paper_info = $paper_arr->fields["paper_info"];
            } else {
                $paper_info = $order_detail_arr[1];
            }
        }
        $member_name_info = $rs->fields["member_name"];
        if ($rs->fields["office_nick"]) {
            $member_name_info .= " <span style=\"color:blue; font-weight: bold;\">[" . $rs->fields["office_nick"] . "]</span>";
        }
        $tr_color = "";

        if($rs->fields["depth1"] == "오늘출고" && $rs->fields["order_state"] != "1180") {
            $tr_color = "style='background: orange !important'";
        } else if($rs->fields["depth1"] == "내일출고" && $rs->fields["order_state"] != "1180") {
            $tr_color = "style='background: lightgreen !important'";
        }
        else if(strpos($rs->fields["cate_sortcode"],"011") !== false && $rs->fields["order_state"] != "1180") {
            $tr_color = "style='background: lightblue !important'";
        }

        if((strpos($rs->fields["cate_sortcode"],"003010001") !== false
                || strpos($rs->fields["cate_sortcode"],"003003002") !== false
                || strpos($rs->fields["cate_sortcode"],"003012001") !== false
                || strpos($rs->fields["cate_sortcode"],"003011001") !== false
                || strpos($rs->fields["cate_sortcode"],"003002003") !== false)
            && $rs->fields["order_state"] != "1180") {
            $tr_color = "style='background: lightblue !important'";
        }

        if($rs->fields["erp_state_name"] == "주문취소") {
            $tr_color = "style='background: lightpink !important'";
        }

        if($rs->fields["erp_state_name"] == "파일에러") {
            $tr_color = "style='background: gold !important'";
        }
        if($rs->fields["erp_state_name"] == "접수에러") {
            $tr_color = "style='background: gold !important'";
        }
        $search_dvs  = $param["search_dvs"];
        if($search_dvs == "'order_mng'") {

            if($rs->fields["erp_state_name"] == "접수완료") {
                $tr_color = "style='background: lightgreen !important'";
            }

            if($rs->fields["erp_state_name"] == "조판대기") {
                $tr_color = "style='background: lightgreen !important'";
            }
        }

        $list_preview_img = "";
        $preview_img = "";
        $j = 0;
        $file_location = "https://orderplatform.s3.ap-northeast-2.amazonaws.com";
        if(startsWith($rs->fields["save_file_name"], "GP") || startsWith($rs->fields["save_file_name"], "DP")){
            $file_location = "";
        }
        if ($rs->fields["preview_file_name"]) {
            foreach (explode('||', $rs->fields["preview_file_name"]) as $value) {
                if ($j > 0) {
                    $preview_img .= "<br>&nbsp;&nbsp;&nbsp;";
                }
                $list_preview_img .= "<img src=\"" . "https://orderplatform.s3.ap-northeast-2.amazonaws.com" . $rs->fields["preview_file_path"] . $value . "\"style=\"max-width:50px;\">";
                $preview_img .= "<img src=\"" . "https://orderplatform.s3.ap-northeast-2.amazonaws.com" . $rs->fields["preview_file_path"] . $value . "\"style=\"max-width:300px;max-height:300px;\">";
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
        if($rs->fields["cate_sortcode"] == "004003009") {
            $order_detail = $order_detail_arr[2] . " / " . $order_detail_arr[0] . " / " . $order_detail_arr[4]
                . " / " . $order_detail_arr[1] . " / " . $order_detail_arr[3];
        }

        if($rework_rs->fields["rework_cause"] != null) {
            $order_detail = "<span style='color:red; font-weight: bold;'>[" . $rework_rs->fields["rework_cause"] . "] </span>" . $order_detail;
        }
        if($rs->fields["file_upload_dvs"] == "R") {
            $order_detail = "<span style='color:red; font-weight: bold;'>[재접수요청] </span>" . $order_detail;
        }
        if($rs->fields["opt_name"] == "당일판") {
            $order_detail = "<span style='color:red; font-weight: bold;'>[당일판] </span>" . $order_detail;
        }
        if($rs->fields["depth1"] == "오늘출고") {
            $order_detail = "<span style='color:red; font-weight: bold;'>[오늘출고] </span>" . $order_detail;
        }
        if($rs->fields["depth1"] == "내일출고") {
            $order_detail = "<span style='color:red; font-weight: bold;'>[내일출고] </span>" . $order_detail;
        }
        if($param['selected_seqno'] == "'".$seqno."'") {
            $temp = array();
            $temp["order_common_seqno"] = $seqno;
            $order_history_arr = $dao->selectOrderHistoryInfo($conn, $temp);
            $order_history = "";
            $inserted_state = array();
            while ($order_history_arr && !$order_history_arr->EOF) {
                $state = $order_history_arr->fields["front_state_name"];
                if($state == "후공정완료") {
                    $state = $order_history_arr->fields["detail"] . "완료";
                }
                if($inserted_state[$state] == 1) {
                    $order_history_arr->MoveNext();
                    continue;
                }
                $inserted_state[$state] = 1;
                $name = $order_history_arr->fields["name"];
                if($name == "") {
                    $name = "미정";
                }
                if($state != "완료")
                    $order_history .= "<div class=\"state-history\"><button type=\"button\" class=\"btn-state\">" . $state . "</button><p>" . date("m.d H:i", strtotime($order_history_arr->fields["state_modi_date"])) . "</p><p>" . $order_history_arr->fields["name"] . " </p></div>";
                $order_history_arr->MoveNext();
            }
            $typset_template = "<a target='_blank' href=\"%s\" download><span class=\"downLink\">%s</span></a>";
            $typset_history_arr = $dao->selectSheetTypsetInfo($conn, $temp);
            $typsets = "";
            $zz = 0;
            while($typset_history_arr && !$typset_history_arr->EOF) {
                $label_info .= sprintf($typset_template, "http://file.gprinting.co.kr" . explode('-', $typset_history_arr->fields["typset_label"])[0] . '-' . explode('-',$typset_history_arr->fields["typset_label"])[1] . "/" . $typset_history_arr->fields["typset_num"] . ".pdf", $typset_history_arr->fields["typset_num"]);
                $typset_info .= sprintf($typset_template, "http://file.gprinting.co.kr" . $typset_history_arr->fields["typset_pdf"], $typset_history_arr->fields["typset_num"]);
                if($zz != 0) $typsets .= ",";
                $typsets .= $typset_history_arr->fields["typset_num"];
                $zz++;
                $typset_history_arr->MoveNext();
            }

            //"<a href=\"%s\" download><span class=\"downLink\">%s</span></a>";
            //"<a href=\"%s\" download><span class=\"downLink\">%s</span></a>";

            $invo_cpn = $order_dlvr_rs->fields["invo_cpn"];
            $invo_num = $order_dlvr_rs->fields["invo_num"];
            $dlvr_pay = 0;
            if(strpos($invo_cpn, "택배") !== false) {
                if($invo_num != "")
                    $invo_num = "송장번호 : " . $invo_num;

                $invo_cpn = $invo_cpn . "(" . number_format($order_dlvr_rs->fields["dlvr_price"]) . "원)";
                if($order_dlvr_rs->fields["dlvr_sum_way"] == "01")  {
                    $invo_cpn .= " 선불택배";
                    $dlvr_pay = $order_dlvr_rs->fields["dlvr_price"];
                }
                else $invo_cpn .= " 후불택배";
            }
        }

        $paper_and_tmpt = $order_detail_arr[3];
        if($rs->fields["cate_sortcode"] == "001001001") {
            $paper_and_tmpt = $order_detail_arr[2] . "<br>" . $order_detail_arr[3];
        } else if(startsWith($rs->fields["cate_sortcode"], "007001")) {
            $paper_and_tmpt = $order_detail_arr[3] . "<br>" . $order_detail_arr[4] . " / " . $order_detail_arr[5] . " / " . $order_detail_arr[6];
        } else if(startsWith($rs->fields["cate_sortcode"], "007002")) {
            $paper_and_tmpt = $order_detail_arr[3] . "<br>" . $order_detail_arr[4] . " / " . $order_detail_arr[5];
        }

        $order_state = $rs->fields["erp_state_name"];
        if(($order_state == "접수대기" || $order_state == "접수중") && $rs->fields["receipt_dvs"] == "Auto")
            $order_state = "접수중";
        //$chk_color = "";

        $after_detail_in = $after_detail;
        if($after_detail_in == "") $after_detail_in = "없음";
        $size_name = $rs->fields["cut_size_wid"] . "x" . $rs->fields["cut_size_vert"] . " (" . $rs->fields["work_size_wid"] . "x" . $rs->fields["work_size_vert"] . ")";
        if(startsWith($rs->fields["cate_sortcode"], "002006")) {
            if($rs->fields["cut_size_wid"] == 390 || $rs->fields["cut_size_wid"] == 400)
                $size_name .= "-(대)";
            if($rs->fields["cut_size_wid"] == 340 || $rs->fields["cut_size_wid"] == 350)
                $size_name .= "-(중)";
            if($rs->fields["cut_size_wid"] == 290 || $rs->fields["cut_size_wid"] == 300)
                $size_name .= "-(소)";

            if($rs->fields["member_seqno"] == 1976) {
                if(strpos($order_detail,"(시트지") === false)
                    $order_detail = str_replace("A형 철재입간판 SET","A형 철재입간판 SET(시트지-사각)",$order_detail);
            } else {
                if(strpos($order_detail,"(자석스티커") === false) {
                    $order_detail = str_replace("A형 철재입간판 SET", "A형 철재입간판 SET(자석스티커-모양)", $order_detail);
                    $order_detail = str_replace("A형 철재입간판 출력물", "A형 철재입간판 출력물(자석스티커-모양)", $order_detail);
                    $order_detail = str_replace("(자석스티커-사각)", "", $order_detail);
                }
                $order_detail = str_replace("(자석스티커-사각)", "(자석스티커-모양)", $order_detail);
            }
        }

        if(trim($order_detail_arr[0]) == "책자"){
            $after_detail = $order_detail;
            $after_detail_in = str_replace("/","<br>",$order_detail);
        }

        //$order_history = $list_order_history;
        $html .= sprintf($html_base, $tr_class
            , $seqno
            , $tr_color
            ,$chk_color
            , $rs->fields["order_num"]
            , $list_preview_img
            , $rs->fields["order_num"]
            , $rs->fields["depo_finish_date"]
            , $rs->fields["state_modi_date"]
            , $order_mng
            , $member_name_info
            , $seqno
            , $title
            , $seqno
            , $order_detail
            , $rs->fields["cut_size_wid"] . "x" . $rs->fields["cut_size_vert"]
            , $after_detail
            , $order_state
            , $str_count
            , ($rs->fields["amt"] * 10 / 10) . $rs->fields["amt_unit_dvs"]
            , number_format($rs->fields["sell_price"])
            , $rs->fields["cust_memo"]
            ,$dlvr_start
            , $dlvr_way
            , $tr_class
            , $seqno
            , $tr_color
            , "판번호 : " . $typsets
            , $list_order_history
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
            , $size_name
            , $rs->fields["title"]
            , $paper_info
            , $rs->fields["amt"] . $rs->fields["amt_unit_dvs"]
            , $str_count
            , $file_location . $rs->fields["file_path"]  . $rs->fields["save_file_name"]
            , $rs->fields["origin_file_name"]
            , $file_size
            , $upload_time
            , $paper_and_tmpt
            , $after_detail_in
            , $opt_detail
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
            , number_format($rs->fields["sell_price"]/1.1)
            , number_format($rs->fields["sell_price"] - $rs->fields["sell_price"]/1.1)
            , number_format($dlvr_pay)
            , number_format($rs->fields["sell_price"] + $dlvr_pay)
            , $order_mng
            , $order_dlvr_info
            , $rs->fields["expec_weight"]
            , $rs->fields["expec_weight"]
            , $seqno
            , $seqno
            , $seqno
            , $seqno
            , $seqno
            , $rs->fields["mail"]
            , $rs->fields["member_name"]
            , $rs->fields["tel_num"]
            , $rs->fields["cell_num"]
            , $order_dlvr_ts->fields["name"]
            , $invo_cpn
            , $invo_num
            , $order_dlvr_rs->fields["name"]
            , $order_dlvr_rs->fields["tel_num"]
            , $order_dlvr_rs->fields["cell_num"]
            , $order_dlvr_rs->fields["zipcode"]
            , $order_dlvr_rs->fields["addr"]
            , $order_dlvr_rs->fields["addr_detail"]
            , $order_mng
            , $seqno
            , $preview_img
            , $rs->fields["accept_file_path"] . $rs->fields["accept_file_name"]
            , $rs->fields["accept_file_name"]
            , $seqno
            , $rs->fields["accept_memo"]
            , $seqno
            , $typset_info
            , $label_info
            , $order_history
            , $order_update_btn
            , $rs->fields["member_seqno"]
            , $seqno
            , $seqno
            ,$rs->fields["page_cnt"]
            ,$rs->fields["count"]
            ,$rs->fields["pay_price"]
            , $rs->fields["member_seqno"]
            , $rs->fields["sell_channel"]
            , $rs->fields["order_num"]
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

function startsWith($haystack, $needle) {
    // search backwards starting from haystack length characters from the end
    return $needle === "" || strrpos($haystack, $needle, -strlen($haystack)) !== false;
}

?>
