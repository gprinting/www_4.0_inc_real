<?
//입금내역 상세보기
function getWithdrawPopup($param) {

    $html = <<<HTML
	<dl>
		<dt class="tit">
		<h4>입금내역 - 상세</h4>
		</dt>
		<dt class="cls">
			<button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
		</dt>
	</dl>
	<div class="pop-base">
	    <div class="pop-content ofa" style="padding:0;">
        <div class="table-body3" style="padding-bottom:10px;text-align: right;">
            <select onchange="changeDvs(this.value);">
                <option value="all">전체</option>
                <option value="bank">가상계좌</option>
                <option value="cash">현금</option>
                <option value="card">카드</option>
                <option value="etc">기타</option>
            </select>
        <div class="table_basic none-hover">
        <table class="table" style="width:100%;">
        <tbody>
            <tr>
                <th>등록일자</th>
                <th>입금유형</th>
                <th>입금액</th>
                <th>입출금 경로상세</th>
                <th>카드사</th>
                <th>카드번호</th>
                <th>승인번호</th>
            </tr>
HTML;

    $html .= $param;

    $html .= <<<HTML
            </tr>
        </tbody>
        </table>
        </div>
        </div>
        </div>
        </div>
HTML;
    return $html;
}

function getReceptCashPopup($param) {

    $html = <<<HTML
	<dl>
		<dt class="tit">
		<h4>현금영수증(대기) - 회원별 상세</h4>
		</dt>
		<dt class="cls">
			<button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
		</dt>
	</dl>
	<div class="pop-base">
	    <div class="pop-content ofa" style="padding:0;">
        <div class="table-body3" style="padding-bottom:10px;text-align: right;">
        <div class="table_basic none-hover">
        <table class="table" style="width:100%;">
        <tbody>
            <tr>
                <th>No.</th>
                <th>채널</th>
                <th>회원명</th>
                <th>주민/핸드폰/사업자번호</th>
                <th>사업자명</th>
                <th>전화번호</th>
                <th>주문번호</th>
                <th>총매출액</th>
                <th>에누리</th>
                <th>순매출액</th>
                <th>발급금액</th>
                <th>변경</th>
                <th>상태</th>
            </tr>
HTML;

    $html .= $param;

    $html .= <<<HTML
            </tr>
        </tbody>
        </table>
        </div>
        </div>
        </div>
        </div>
HTML;
    return $html;
}

function getCardpayPopup($param) {

    $html = <<<HTML
	<dl>
		<dt class="tit">
		<h4>카드결제 - 회원별 상세</h4>
		</dt>
		<dt class="cls">
			<button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
		</dt>
	</dl>
	<div class="pop-base">
	    <div class="pop-content ofa" style="padding:0;">
        <div class="table-body3" style="padding-bottom:10px;text-align: right;">
        <div class="table_basic none-hover">
        <table class="table" style="width:100%;">
        <tbody>
            <tr>
                <th>No.</th>
                <th>채널</th>
                <th>별칭회원명</th>
                <th>정식회원명</th>
                <th>전화번호</th>
                <th>주문번호</th>
                <th>인쇄물제목</th>
                <th>총매출액</th>
                <th>에누리</th>
                <th>순매출액</th>
                <th>발급금액</th>
                <th>변경</th>
                <th>상태</th>
            </tr>
HTML;

    $html .= $param;

    $html .= <<<HTML
            </tr>
        </tbody>
        </table>
        </div>
        </div>
        </div>
        </div>
HTML;
    return $html;
}


//입금내역 상세보기
function getTotalDayPopup($param) {

    $html = <<<HTML
	<dl>
		<dt class="tit">
		<h4>입금내역 - 상세</h4>
		</dt>
		<dt class="cls">
			<button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
		</dt>
	</dl>
	<div class="pop-base">
	    <div class="pop-content ofa" style="padding:0;">
        <div class="table-body3" style="padding-bottom:0;">
        <div class="table_basic none-hover">
        <table class="table" style="width:100%;">
        <tbody>
            <tr>
                <th>일자</th>
                <th>총 매출</th>
                <th>조정금액</th>
                <th>입금액(가상계좌)</th>
                <th>입금액(카드)</th>
            </tr>
HTML;

    $html .= $param;

    $html .= <<<HTML
            </tr>
        </tbody>
        </table>
        </div>
        </div>
        </div>
        </div>
HTML;
    return $html;
}

function getMemoPopup($param) {

    $html = <<<HTML
	<dl>
		<dt class="tit">
		<h4>메모 - 상세</h4>
		</dt>
		<dt class="cls">
			<button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
		</dt>
	</dl>
	<div class="pop-base">
	     <table class="table fix_width100f">
             <tr>
                 <!--th style="background: #eee;">발주업체</th>
                 <td id="company_cnt">0원</td-->
                 <th style="background: #eee;">체크일자</th>
                 <td>
                    <input id="memo_date" class="memo_date" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" id="date_from2" name="date_from">
                </td>
             </tr>
             <tr>
                 <th style="background: #eee;">내용</th>
                 <td>
                    <input type="textarea" id="member_memo" style="width: 90%;height: 100px;"/>
                 </td>
             </tr>
         </table>
         <div style="text-align: right;margin-right: 90px;margin-top: 20px;">
            <button type="button" class='btn_Turquoise01' style="width: 80px;" class="" onclick="writeMemo('$param');">입력</button>
            <button type="button" class='btn_Turquoise01' style="width: 80px;" class="" onclick="hideRegiPopup();">닫기</button>
         </div>
    </div>
HTML;
    return $html;
}



function getDepositPopup($param) {
    $html = <<<HTML
	<dl>
		<dt class="tit">
		<h4>입금 관리</h4>
		</dt>
		<dt class="cls">
			<button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
		</dt>
	</dl>
		<div class="pop-base">
	    <div class="pop-content ofa" style="padding:0;">
        <div class="table-body3" style="padding-bottom:0;">
        <div class="table_basic none-hover">
        <table class="table fix_width100f">
             <tr>
                 <!--th style="background: #eee;">발주업체</th>
                 <td id="company_cnt">0원</td-->
                 <th style="background: #eee;">입금일자</th>
                 <td>
                    <input id="deposit_from" class="deposit_from" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="deposit_from">
                    ~<input id="deposit_to" class="deposit_to" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="deposit_to">
                    <button type="button" class='btn_Turquoise01' style="width: 80px;" class="" onclick="searchDeposit();">검색</button>
                </td>
             </tr>
         </table>
         <input type="hidden" name="active_form" id="active_form" value="bank">
         <form name="adjust_form" id="adjust_form" onsubmit="return false;">
			<table class="table_search_layout">
				<tbody>
				    <tr>
						<th class="th_text calcul_mng_th_left">거래처명</th>
						<td>
							<input type="text" disabled class="input_co2 fix_width150" name="member_name" id="member_name">
						</td>
						<th class="th_text calcul_mng_th_left">입금일자</th>
						<td>
							<input id="deposit_date" class="date" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="deposit_date">
						</td>
						<th class="th_text calcul_mng_th_left">입금방법</th>
						<td>
							<select id="dvs" name="dvs" onchange="change_dvs(this.value);" class="fix_width100">
							    <option value="bank">가상계좌</option>
                                <option value="cash">현금</option>
                                <option value="card">카드(단말기)</option>
                                <option value="etc">기타</option>
							</select>
						</td>
					</tr>
				</tbody>
			</table>
			</form>
			<form name="bank_form" id="bank_form" onsubmit="return false;">
			<table class="table_search_layout">
				<tbody>
					<tr>
						<th class="th_text calcul_mng_th_left">입금액</th>
						<td>
							<input type="text" id="bank_price" name="bank_price"
							onkeydown="return checkOnlyNumber(event)" 
							onkeyup="removeChar(event)" maxlength="18" 
							style="ime-mode:disabled;" class="input_co2 fix_width182">
						</td>
						<th class="th_text calcul_mng_th_left">입금자명</th>
						<td>
							<input type="text" class="input_co2 fix_width150" name="bank_name" id="bank_name">
						</td>
					</tr>
					<tr colspan="3">
						<th class="th_text calcul_mng_th_left">비고</th>
						<td>
							<input type="text" class="input_co2 fix_width150" name="bank_cont" id="bank_cont">
						</td>
					</tr>
				</tbody>
			</table>
		</form>
         <form name="cash_form" id="cash_form" onsubmit="return false;" style="display:none;">
			<table class="table_search_layout">
				<tbody>
					<tr>
						<th class="th_text calcul_mng_th_left">현금</th>
						<td>
							<input type="text" id="cash_price" name="cash_price"
							onkeydown="return checkOnlyNumber(event)" 
							onkeyup="removeChar(event)" maxlength="18" 
							style="ime-mode:disabled;" class="input_co2 fix_width182">
						</td>
						<th class="th_text calcul_mng_th_left">현금구분</th>
						<td>
							<select id="cash_dvs" name="cash_dvs" class="fix_width100">
                                <option value="큰금고">큰금고</option>
                                <option value="굿프린팅">굿프린팅</option>
                                <option value="디지털프린팅">디지털프린팅</option>
							</select>
						</td>
					</tr>
					<tr>
						<th class="th_text calcul_mng_th_left">비고</th>
						<td>
							<input type="text" class="input_co2 fix_width150" name="cash_memo" id="cash_memo">
						</td>
					</tr>
				</tbody>
			</table>
		</form>
		<form name="card_form" id="card_form" onsubmit="return false;" style="display:none;">
			<table class="table_search_layout">
				<tbody>
					<tr>
						<th class="th_text calcul_mng_th_left">카드금액</th>
						<td>
							<input type="text" id="card_price" name="card_price"
							onkeydown="return checkOnlyNumber(event)" 
							onkeyup="removeChar(event)" maxlength="18" 
							style="ime-mode:disabled;" class="input_co2 fix_width182">
						</td>
						<th class="th_text calcul_mng_th_left">카드사</th>
						<td>
							<select id="card_kind" name="card_kind" class="fix_width100">
                                <option value="외환">외환</option>
                                <option value="국민">국민</option>
                                <option value="BC">BC</option>
                                <option value="LG">LG</option>
                                <option value="신한">신한</option>
                                <option value="하나">하나</option>
                                <option value="삼성">삼성</option>
                                <option value="평화">평화</option>
                                <option value="현대">현대</option>
                                <option value="국민">국민</option>
                                <option value="한미">한미</option>
                                <option value="조흥">조흥</option>
                                <option value="한미">한미</option>
                                <option value="수협">수협</option>
                                <option value="롯데">롯데</option>
                                <option value="신세계">신세계</option>
							</select>
						</td>
						<th class="th_text calcul_mng_th_left">할부개월수</th>
						<td>
						    <input type="text" class="input_co2 fix_width150" name="card_inst_months" id="card_inst_months">
						</td>
					</tr>
					<tr>
						<th class="th_text calcul_mng_th_left">카드번호</th>
						<td>
						    <input type="text" class="input_co2 fix_width150" name="card_num" id="card_num">
						</td>
						<th class="th_text calcul_mng_th_left">승인번호</th>
						<td>
							<input type="text" class="input_co2 fix_width150" name="card_approve_num" id="card_approve_num">
						</td>
					</tr>
					<tr>
						<th class="th_text calcul_mng_th_left">승인일자</th>
						<td>
							<input placeholder="yyyy-MM-dd" class="input_co2 fix_width150 deposit_to" id="card_approve_date" name="card_approve_date" readonly="">
						</td>
						<th class="th_text calcul_mng_th_left">상호</th>
						<td>
							<input type="text" class="input_co2 fix_width150" name="card_member" id="card_member">
						</td>
					</tr>
					<tr>
						<th class="th_text calcul_mng_th_left">비고</th>
						<td>
							<input type="text" class="input_co2 fix_width150" name="card_memo" id="card_memo">
						</td>
					</tr>
				</tbody>
			</table>
		</form>
		<form name="etc_form" id="etc_form" onsubmit="return false;" style="display:none;">
			<table class="table_search_layout">
				<tbody>
					<tr>
						<th class="th_text calcul_mng_th_left">기타</th>
						<td>
						    <input type="text" id="etc_price" name="etc_price"
						    	onkeydown="return checkOnlyNumber(event)" 
						    	onkeyup="removeChar(event)" maxlength="18" 
						    	style="ime-mode:disabled;" class="input_co2 fix_width182">
						</td>
					</tr>
					<tr>
						<th class="th_text calcul_mng_th_left">기타구분</th>
						<td>
							<select id="etc_dvs" name="etc_dvs" class="fix_width250">
                                <option value="입금 미입력 확인분">입금 미입력 확인분</option>
                                <option value="타업체입력오기">타업체입력오기</option>
                                <option value="금액환불">금액환불</option>
                                <option value="거래처이전">거래처이전</option>
                                <option value="현금입금처리">현금입금처리</option>
							</select>
						</td>
					</tr>
					<tr>
						<th class="th_text calcul_mng_th_left">비고</th>
						<td>
							<input type="text" class="input_co2 fix_width250" name="etc_cont" id="etc_cont">
						</td>
					</tr>
				</tbody>
			</table>
		</form>
		<div class="btn_wrapper">
				<button id="save_btn" class="btn btn_orange fix_width120 fix_height30 co_blue fs12" onclick="saveAdjust();">저장</button>
				<button style="display:none;" id="del_btn" class="btn btn_red fix_width120 fix_height30 co_blue fs12" onclick="delAdjust();">삭제</button>
				<button style="display:none;" id="cancle_btn" class="btn btn_blue fix_width120 fix_height30 co_blue fs12" onclick="resetRegiForm();">보기취소</button>
			</div>
		<div style="text-align: end;">
		    <select id="dvs2" onchange="change_dvs2(this.value);" class="fix_width100">
		        <option value="all">전체</option>
		        <option value="bank">가상계좌</option>
                <option value="cash">현금</option>
                <option value="card">카드</option>
                <option value="etc">기타</option>
            </select>
        </div>
        <table class="table" style="width:100%;margin-top:20px;">
        <tbody id="deposit_list">
            <tr>
                <th>입금일자</th>
                <th>입금금액</th>
                <th>입금경로</th>
                <th>입금자</th>
                <th>카드 번호</th>
                <th>할부개월</th>
                <th>승인번호</th>
                <th>담당자</th>
            </tr>
HTML;

    $html .= $param;

    $html .= <<<HTML
            </tr>
        </tbody>
        </table>
        </div>
        </div>
        </div>
        </div>
HTML;
    return $html;
}


function getPointPopup($param) {
    $html = <<<HTML
	<dl>
		<dt class="tit">
		<h4>출석체크 관리</h4>
		</dt>
		<dt class="cls">
			<button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
		</dt>
	</dl>
		<div class="pop-base">
	    <div class="pop-content ofa" style="padding:0;">
        <div class="table-body3" style="padding-bottom:0;">
        <div class="table_basic none-hover">
        <table class="table fix_width100f">
             <tr>
                 <!--th style="background: #eee;">발주업체</th>
                 <td id="company_cnt">0원</td-->
                 <th style="background: #eee;">조회일자</th>
                 <td>
                    <input id="deposit_from" class="deposit_from" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="deposit_from">
                    ~<input id="deposit_to" class="deposit_to" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="deposit_to">
                    <!--button type="button" class='btn_Turquoise01' style="width: 80px;" class="" onclick="searchDeposit();">검색</button-->
                </td>
             </tr>
         </table>
         <input type="hidden" name="active_form" id="active_form" value="bank">
         <form name="adjust_form" id="adjust_form" onsubmit="return false;">
			<table class="table_search_layout">
				<tbody>
				    <tr>
						<th class="th_text calcul_mng_th_left">거래처명</th>
						<td>
							<input type="text" disabled class="input_co2 fix_width150" name="member_name" id="member_name">
						</td>
						<!--th class="th_text calcul_mng_th_left">입금일자</th>
						<td>
							<input id="deposit_date" class="date" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="deposit_date">
						</td>
						<th class="th_text calcul_mng_th_left">입금방법</th>
						<td>
							<select id="dvs" name="dvs" onchange="change_dvs(this.value);" class="fix_width100">
							    <option value="bank">가상계좌</option>
                                <option value="cash">현금</option>
                                <option value="card">카드(단말기)</option>
                                <option value="etc">기타</option>
							</select>
						</td-->
					</tr>
				</tbody>
			</table>
			</form>
			
        <table class="table" style="width:100%;margin-top:20px;">
        <tbody id="deposit_list">
            <tr>
                <th>기타</th>
                <th>포인트</th>
                <th>일자</th>
				<th>개근일자</th>
            </tr>
HTML;

    $html .= $param;

    $html .= <<<HTML
            </tr>
        </tbody>
        </table>
        </div>
        </div>
        </div>
        </div>
HTML;
    return $html;
}

function getPointUsePopup($param) {
    $html = <<<HTML
	<dl>
		<dt class="tit">
		<h4>포인트사용내역</h4>
		</dt>
		<dt class="cls">
			<button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
		</dt>
	</dl>
		<div class="pop-base">
	    <div class="pop-content ofa" style="padding:0;">
        <div class="table-body3" style="padding-bottom:0;">
        <div class="table_basic none-hover">
        <table class="table fix_width100f">
             <tr>
                 <!--th style="background: #eee;">발주업체</th>
                 <td id="company_cnt">0원</td-->
                 <th style="background: #eee;">조회일자</th>
                 <td>
                    <input id="deposit_from" class="deposit_from" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="deposit_from">
                    ~<input id="deposit_to" class="deposit_to" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="deposit_to">
                    <!--button type="button" class='btn_Turquoise01' style="width: 80px;" class="" onclick="searchDeposit();">검색</button-->
                </td>
             </tr>
         </table>
         <input type="hidden" name="active_form" id="active_form" value="bank">
         <form name="adjust_form" id="adjust_form" onsubmit="return false;">
			<table class="table_search_layout">
				<tbody>
				    <tr>
						<th class="th_text calcul_mng_th_left">거래처명</th>
						<td>
							<input type="text" disabled class="input_co2 fix_width150" name="member_name" id="member_name">
						</td>
						<!--th class="th_text calcul_mng_th_left">입금일자</th>
						<td>
							<input id="deposit_date" class="date" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="deposit_date">
						</td>
						<th class="th_text calcul_mng_th_left">입금방법</th>
						<td>
							<select id="dvs" name="dvs" onchange="change_dvs(this.value);" class="fix_width100">
							    <option value="bank">가상계좌</option>
                                <option value="cash">현금</option>
                                <option value="card">카드(단말기)</option>
                                <option value="etc">기타</option>
							</select>
						</td-->
					</tr>
				</tbody>
			</table>
			</form>
			
        <table class="table" style="width:100%;margin-top:20px;">
        <tbody id="deposit_list">
            <tr>
                <th>일자</th>
                <th>주문번호</th>
                <th>제목</th>
				<th>주문금액</th>
				<th>사용포인트</th>
				<th>결제금액</th>

            </tr>
HTML;

    $html .= $param;

    $html .= <<<HTML
            </tr>
        </tbody>
        </table>
        </div>
        </div>
        </div>
        </div>
HTML;
    return $html;
}

function getEnuriPopup($param) {

    $html = <<<HTML
	<dl>
		<dt class="tit">
		<h4>에누리 관리</h4>
		</dt>
		<dt class="cls">
			<button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
		</dt>
	</dl>
		<div class="pop-base">
	    <div class="pop-content ofa" style="padding:0;">
        <div class="table-body3" style="padding-bottom:0;">
        <div class="table_basic none-hover">
        <table class="table fix_width100f">
             <tr>
                 <!--th style="background: #eee;">발주업체</th>
                 <td id="company_cnt">0원</td-->
                 <th style="background: #eee;">에누리일자</th>
                 <td>
                    <input id="enuri_from" class="enuri_from" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="enuri_from">
                    ~<input id="enuri_to" class="enuri_to" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="enuri_to">
                    <button type="button" class='btn_Turquoise01' style="width: 80px;" class="" onclick="searchEnuri();">검색</button>
                </td>
             </tr>
         </table>
         <form name="enuri_form" id="enuri_form" onsubmit="return false;">
			<table class="table_search_layout">
				<tbody>
				    <tr>
				        <th class="th_text calcul_mng_th_left">거래처명</th>
						<td>
							<input type="text" disabled class="input_co2 fix_width150" name="member_name" id="member_name">
						</td>
						<th class="th_text calcul_mng_th_left">차감일자</th>
						<td>
							<input id="enuri_date" class="date" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="enuri_date">
						</td>
                    </tr>
					<tr>
					    <th class="th_text calcul_mng_th_left">주문번호</th>
						<td>
							<input type="text" class="input_co2 fix_width150" name="order_num" id="order_num">
						</td>
						<th class="th_text calcul_mng_th_left">차감액</th>
						<td>
						    <input type="text" id="enuri_price" name="enuri_price"
						    	onkeydown="return checkOnlyNumber(event)" 
						    	onkeyup="removeChar(event)" maxlength="18" 
						    	style="ime-mode:disabled;" class="input_co2 fix_width182">
						</td>
					</tr>
					<tr>
						<th class="th_text calcul_mng_th_left">비고</th>
						<td>
							<input type="text" class="input_co2 fix_width400" name="enuri_memo" id="enuri_memo">
						</td>
					</tr>
				</tbody>
			</table>
		</form>
		<div class="btn_wrapper">
				<button id="save_btn" class="btn btn_orange fix_width120 fix_height30 co_blue fs12" onclick="saveEnuri();">저장</button>
			</div>
        <table class="table" style="width:100%;margin-top:30px;">
        <tbody id="enuri_list">
HTML;

    $html .= $param;

    $html .= <<<HTML
            </tr>
        </tbody>
        </table>
        </div>
        </div>
        </div>
        </div>
HTML;
    return $html;
}

function getPaypricePopup($param) {

    $html = <<<HTML
	<dl>
		<dt class="tit">
		<h4>매출 관리</h4>
		</dt>
		<dt class="cls">
			<button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
		</dt>
	</dl>
		<div class="pop-base">
	    <div class="pop-content ofa" style="padding:0;">
        <div class="table-body3" style="padding-bottom:0;">
        <div class="table_basic none-hover">
        <table class="table fix_width100f">
             <tr>
                 <th style="background: #eee;">매출일자</th>
                 <td>
                    <input id="payprice_from" class="payprice_from" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="enuri_from">
                    ~<input id="payprice_to" class="payprice_to" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="enuri_to">
                    <button type="button" class='btn_Turquoise01' style="width: 80px;" class="" onclick="searchPayprice();">검색</button>
                </td>
             </tr>
         </table>
         <form name="pay_form" id="pay_form" onsubmit="return false;">
			<table class="table_search_layout">
				<tbody>
				    <tr>
				        <th class="th_text calcul_mng_th_left">거래처명</th>
						<td>
							<input type="text" disabled class="input_co2 fix_width150" name="member_name" id="member_name">
						</td>
						<th class="th_text calcul_mng_th_left">차감일자</th>
						<td>
							<input id="payprice_date" class="date" placeholder="yyyy-MM-dd" class="input_co2 fix_width70 date" name="payprice_date">
						</td>
                    </tr>
					<tr>
						<th class="th_text calcul_mng_th_left">매출액</th>
						<td>
						    <input type="text" id="payprice_price" name="payprice_price"
						    	onkeydown="return checkOnlyNumber(event)" 
						    	onkeyup="removeChar(event)" maxlength="18" 
						    	style="ime-mode:disabled;" class="input_co2 fix_width182">
						</td>
						<th class="th_text calcul_mng_th_left">적요</th>
						<td>
							<input type="text" class="input_co2 fix_width400" name="payprice_memo" id="payprice_memo">
						</td>
					</tr>
				</tbody>
			</table>
		</form>
		<div class="btn_wrapper">
				<button id="save_btn" class="btn btn_orange fix_width120 fix_height30 co_blue fs12" onclick="savePayprice();">저장</button>
			</div>
        <table class="table" style="width:100%;margin-top:30px;">
        <tbody id="payprice_list">
HTML;

    $html .= $param;

    $html .= <<<HTML
            </tr>
        </tbody>
        </table>
        </div>
        </div>
        </div>
        </div>
HTML;
    return $html;
}
?>
