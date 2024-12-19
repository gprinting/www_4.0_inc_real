<?
//출력 상세보기
function getOutputDetailPopup($param) {

    $html = <<<HTML
    <div class="modal_title">
        <span class="modal_title_span">판정보</span>
    </div>
    <a class="close_popup" onclick="hideRegiPopup();">x</a>

	<div>
		<div class="" style="padding:0;">
        <div class="" style="padding:20px 40px;">
        <ul class="table_top">
			<li style="float:left;list-style: none;padding-bottom: 10px;">
			</li>
            <li style="float:right;list-style: none;padding-bottom: 10px;">
                $param[btn_html]
            </li>
        </ul>
        <div class="">
            <table class="table_basic_layout_popup">
                <colgroup>
                    <col style="width:150px;">
                    <col style="width:300px;">
                    <col style="width:150px;">
                    <col style="width:300px;">
                </colgroup>
                <tbody>
                    <tr>
                        <th class="th_text">조판번호</th>
                        <td><input type='text' value='$param[typset_num]' disabled /></td>
                        <th class="th_text">출력명</th>
                        <td>
                        $param[print_title]
                        <input type="hidden" id="output_name" value="$param[output_name]">
                        </td>
                    </tr>
                    <tr>
                        <th class="th_text">운영체제</th>
                        <td>$param[oper_sys]</td>
                        <th class="th_text">사이즈</th>
                        <td>
                        $param[affil] $param[subpaper] ($param[size])
                        <input type="hidden" id="size" value="$param[size]">
                        </td>
                    </tr>
                    <tr>
                        <th class="th_text">인쇄 도수</th>
                        <td>
                            전면 기본 $param[beforeside_tmpt]도 별색 $param[beforeside_spc_tmpt]도<br />
                            후면 기본 $param[aftside_tmpt]도 별색 $param[aftside_spc_tmpt]도 $param[honggak]
                        </td>
                        <th class="th_text">후공정</th>
                        <td>
                            $param[after_list]
                        </td>
                    </tr>
                    <tr>
                        <th class="th_text">조판자</th>
                        <td>$param[orderer]</td>
                        <th class="th_text">마감구분</th>
                        <td>$param[dlvrboard]</td>
                    </tr>
                    <tr>
                        <th class="th_text">판구분</th>
                        <td>
                            <select class="fix_width150" style="width:90%;" id="board_dvs" onchange="getWorkPrice();">
                                $param[board_dvs]
                            </select>
                        </td>
                        <th class="th_text">판 수량</th>
                        <td>
                            <input type="text" class="input_output_pop_detail_01" style="text-align:right;" id="amt" value="$param[amt]" onkeyup="getWorkPrice();" $param[disabled]> 판
                        </td>
                    </tr>
                    <tr>
                        <th class="th_text">메모(특이사항)</th>
                        <td colspan="3"><span style="color:red;">$param[specialty_items]</span></td>
                    </tr>
                    <tr>
                        <th class="th_text">작업자</th>
                        <td colspan="3">
                        <select class="select_output_pop_detail_01" onchange="changeManu(this.value);" id="extnl_etprs_seqno" $param[disabled]>
                            $param[manu_html]
                        </select>
                        <select class="select_output_pop_detail_01" onchange="getWorkPrice();" id="extnl_brand_seqno" $param[disabled]>
                            $param[brand_html]
                        </select>
                        $param[worker]
                        </td>
                    </tr>
                    <tr>
                        <th class="th_text">작업자 메모</th>
                        <td colspan="3">
                            $param[memo_select_html] 
                        </td>
                    </tr>
                    <tr>
                        <th class="th_text">작업금액</th>
                        <td>
                            <span id="work_price_val">$param[price]원</span>
                            <input type="hidden" value="$param[price]" id="work_price">
                        </td>
                        <th class="th_text">조정금액</th>
                        <td><input type="text" class="input_output_pop_detail_01" style="text-align:right;" id="adjust_price" value="$param[adjust_price]" $param[disabled] onkeyup="getNumberFormat(this.value, 'adjust_price');"> 원
                        </td>
                    </tr>
                    <tr>
                        <th class="th_text">작업시각</th>
                        <td>$param[work_start_hour] $param[work_end_hour]</td>
                        <th class="th_text">인쇄소</th>
                        <td>
                            <select class="fix_width150" id="extnl_etprs" onchange="extnl_etprs_changed('$param[sheet_typset_seqno]', this);">
                            $param[extnl_etprs]
                            </select>
                        </td>
                    </tr>
                    <!--tr>
                        <td colspan="4" style="text-align: center;">
                            $param[pic]
                        </td>
                    </tr-->
                </tbody>
            </table>
         </div>
	     </div> <!-- pop-content -->

         <!--div class="table-body3" style="padding-top:0;">
	            <ul class="table_top">
				 	 <li class="sel">
                          전체사양
				     </li>
				     <li class="sel tar" style="width:100%;">
				     </li>
	            </ul>
                <table class="table" style="width:100%;">
                    <tbody>
                        <tr>
                            <th>종이</th>
                            <td>$param[paper_name] $param[paper_dvs] $param[paper_color] $param[paper_basisweight]</td>
                            <th>발주처</th>
                            <td></td>
                        </tr>
                        <tr>
                            <th>인쇄</th>
                            <td>
                                전면 기본 $param[beforeside_tmpt]도 별색 $param[beforeside_spc_tmpt]도 / 후면 기본 $param[aftside_tmpt]도 별색 $param[aftside_spc_tmpt]도
                            </td>
                            <th>발주처</th>
                            <td></td>
                        </tr>
                    </tbody>
                </table>
         </div-->

         <!--div class="table-body" style="padding-top:0;">
	            <ul class="table_top">
				 	  <li class="sel">
				     </li>
				     <li class="sel tar" style="width:100%;">
                        $param[finish_modi]
				     </li>
	            </ul>
                <table class="table" style="width:100%;">
                    <thead>
                        <tr>
                            <th>작업시간</th>
                            <th>작업메모</th>
                            <th>유효여부</th>
                            <th>작업자</th>
                            <th>상태</th>
                        </tr>
                    </thead>
                    <tbody>
                        $param[work_list]
                    </tbody>
                </table>
         </div-->
	</div>
HTML;

    return $html;
}

//종이발주 상세보기
function getPaperOpDetailPopup($param) {

    $html = <<<HTML
    <div class="modal_title">
        <span class="modal_title_span">종이발주</span>
    </div>
    <a class="close_popup" onclick="hideRegiPopup();">x</a>

	<div>
		<div class="" style="padding:0;">
        <div class="" style="padding:20px 40px;">
        <ul class="table_top">
			<li style="float:left;list-style: none;padding-bottom: 10px;">
			</li>
            <li style="float:right;list-style: none;padding-bottom: 10px;">
                $param[btn_html]
            </li>
        </ul>
        <div class="">
            <table class="table_basic_layout_popup">
                <colgroup>
                    <col style="width:150px;">
                    <col style="width:300px;">
                    <col style="width:150px;">
                    <col style="width:300px;">
                </colgroup>
                <tbody>
                    <tr>
                        <th class="th_text">조판번호</th>
                        <td colspan="3"><input type='text' id='typset_num1' value='$param[typset_num]' disabled /></td>
                    </tr>
                    <tr>
                        <th class="th_text">발주사이즈</th>
                        <td>
                            <input type="hidden" id="size" value="$param[size]" />
                            <select class="fix_width150" style="width:90%;" id="op_size">
                                <option>국전 910X650(9개)</option>
                                <option>국전 880X625</option>
                                <option>국반 636X469</option>
                                <option>국전 939x636</option>
                                <option>국전 636x939</option>
                                <option>국2절 636x469</option>
                                <option>국4절 465X317</option>
                                <option>2절 939X720(13개)</option>
                                <option>국전 930X660(12개반)</option>
                                <option>국3절 636 X 313</option>
                                <option>2절 788X650(10개)</option>
                                <option>2절 788X570(9개)</option>
                                <option>2절 788X545</option>
                                <option>2절 545x788</option>
                                <option>2절 760X545(8개)</option>
                                <option>4절 545X394</option>
                                <option>3절 788X363</option>
                                <option>장6 545X260</option>
                                <option>T3 679X393</option>
                                <option>46전지 788X1091</option>
                                <option>8절 394X272</option>
                                <option>3절 655X363(리플렛)</option>
                                <option>2절 788X506(대봉)</option>
                                <option>2절 720X545(소봉)</option>
                                <option>2절 1060X740(소봉 12개)</option>
                                <option>2절 1020X670(대소봉6개)</option>
                                <option>2절 788X506(대봉+소봉2개)</option>
                                <option>930 X 715(13개반)</option>
                                <option>2절 930X715(횡결)</option>
                                <option>2절 545X788(대봉)</option>
                                <option>국전 1060X650(10개)</option>
                                <option>2절 1060X750(16개)</option>
                            </select>
                        </td>
                        <th class="th_text">입고사이즈</th>
                        <td>
                            <select class="fix_width150" style="width:90%;" id="stor_size">
                                <option>국전 910X650(9개)</option>
                                <option>국전 880X625</option>
                                <option>국반 636X469</option>
                                <option>국전 939X636</option>
                                <option>국전 939x636</option>
                                <option>국전 636x939</option>
                                <option>국2절 636x469</option>
                                <option>국4절 465X317</option>
                                <option>2절 939X720(13개)</option>
                                <option>국전 930X660(12개반)</option>
                                <option>국3절 636 X 313</option>
                                <option>2절 788X650(10개)</option>
                                <option>2절 788X570(9개)</option>
                                <option>2절 788X545</option>
                                <option>2절 545x788</option>
                                <option>2절 760X545(8개)</option>
                                <option>4절 545X394</option>
                                <option>3절 788X363</option>
                                <option>장6 545X260</option>
                                <option>T3 679X393</option>
                                <option>46전지 788X1091</option>
                                <option>8절 394X272</option>
                                <option>3절 655X363(리플렛)</option>
                                <option>2절 788X506(대봉)</option>
                                <option>2절 720X545(소봉)</option>
                                <option>2절 1060X740(소봉 12개)</option>
                                <option>2절 1020X670(대소봉6개)</option>
                                <option>2절 788X506(대봉+소봉2개)</option>
                                <option>930 X 715(13개반)</option>
                                <option>2절 930X715(횡결)</option>
                                <option>2절 545X788(대봉)</option>
                                <option>국전 1060X650(10개)</option>
                                <option>2절 1060X750(16개)</option>
                            </select>
                        </td>
                    </tr>
                    <tr>
                        <th class="th_text">종이</th>
                        <td>
                            <input type="text" class="input_output_pop_detail_01" style="text-align:left;width:250px;" id="paper_name" value="$param[paper_name]">
                        </td>
                        <th class="th_text">발주처</th>
                        <td>
                            <select class="fix_width150" style="width:90%;" id="brand_seqno">
                                $param[paper_manu_html]
                            </select>
                        </td>
                        <!--th class="th_text">결</th>
                        <td>
                            <label class="check_box_wrapper_paper_op"><input type="radio" class="checkbox_align" name="paper_grain" value="종목" checked="checked">종목</label>
                            <label class="check_box_wrapper_paper_op"><input type="radio" class="checkbox_align" name="paper_grain" value="횡목">횡목</label>
                        </td-->
                    </tr>
                    <tr>
                        <th class="th_text">수량</th>
                        <td>
                            <input type="text" class="input_output_pop_detail_01" style="text-align:right;" id="paper_amt" value="$param[print_amt]">
                            <label>매</label>
                        </td>
                        <th class="th_text">인쇄소</th>
                        <td>
                            <input type="text" id="storplace" value="$param[print_etprs]" disabled/>
                        </td>
                    </tr>
                    <tr>
                        <th class="th_text">메모</th>
                        <td colspan="3"><input type="text" class="input_output_pop_detail_01" style="text-align:left;width:90%;" id="paper_memo" value=""></td>
                    </tr>
                    <!--tr>
                        <td colspan="4" style="text-align: center;">
                            $param[pic]
                        </td>
                    </tr-->
                </tbody>
            </table>
            <div style='text-align: right;'>
                <button type="button" id="btn_complete" class="btn_Turquoise01 h_26" onclick="toPaperOp('$param[sheet_typset_seqno]')">발주</button>
            </div>
         </div>
	     </div> <!-- pop-content -->

         <!--div class="table-body3" style="padding-top:0;">
	            <ul class="table_top">
				 	 <li class="sel">
                          전체사양
				     </li>
				     <li class="sel tar" style="width:100%;">
				     </li>
	            </ul>
                <table class="table" style="width:100%;">
                    <tbody>
                        <tr>
                            <th>종이</th>
                            <td>$param[paper_name] $param[paper_dvs] $param[paper_color] $param[paper_basisweight]</td>
                            <th>발주처</th>
                            <td></td>
                        </tr>
                        <tr>
                            <th>인쇄</th>
                            <td>
                                전면 기본 $param[beforeside_tmpt]도 별색 $param[beforeside_spc_tmpt]도 / 후면 기본 $param[aftside_tmpt]도 별색 $param[aftside_spc_tmpt]도
                            </td>
                            <th>발주처</th>
                            <td></td>
                        </tr>
                    </tbody>
                </table>
         </div-->

         <!--div class="table-body" style="padding-top:0;">
	            <ul class="table_top">
				 	  <li class="sel">
				     </li>
				     <li class="sel tar" style="width:100%;">
                        $param[finish_modi]
				     </li>
	            </ul>
                <table class="table" style="width:100%;">
                    <thead>
                        <tr>
                            <th>작업시간</th>
                            <th>작업메모</th>
                            <th>유효여부</th>
                            <th>작업자</th>
                            <th>상태</th>
                        </tr>
                    </thead>
                    <tbody>
                        $param[work_list]
                    </tbody>
                </table>
         </div-->
	</div>
HTML;

    return $html;
}

function getOutputLabelPopup($param) {
    $html = <<<HTML
    <div class="modal_title">
        <span class="modal_title_span">출력공정 - 딱판이미지</span>
    </div>
    <a class="close_popup" onclick="hideRegiPopup();">x</a>

    <div style="padding: 20px 40px;">
    <li style="float:right;list-style: none;padding-left: 10px;">
    <button type="button" id="btn_complete" class="btn_yellow_80 h_26" onclick="downloadURL('$param[downloadurl]')">다운로드</button>
    </li>
	<div class="pop-base">
		<div class="pop-content ofa" style="padding:0;">
        <div class="table_basic none-hover">
            <div class="demo">
                <div class="item">
                    <div class="clearfix">
                        <ul id="image-gallery" class="gallery list-unstyled cS-hidden">
                            $param[pic]
                        </ul>
                    </div>
                </div>
            </div>
        </div>
	</div>
	</div>
	</div>
HTML;

    return $html;
}

//출력 이미지보기
function getOutputImgPopup($param) {

    $html = <<<HTML
    <div class="modal_title">
        <span class="modal_title_span">출력공정 - 조판이미지</span>
    </div>
    <a class="close_popup" onclick="hideRegiPopup();">x</a>

	<div class="pop-base">
		<div class="pop-content ofa" style="padding:0;">
        <div class="table_basic none-hover">
            <div class="demo">
                <div class="item">            
                    <div class="clearfix">
                        <ul id="image-gallery" class="gallery list-unstyled cS-hidden">
                            $param[pic]
                        </ul>
                    </div>
                </div>
            </div>
        </div>
	</div>
HTML;

    return $html;
}
?>
