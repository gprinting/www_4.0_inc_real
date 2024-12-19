<?
/**
 * @brief 견적 등록 html 생성
 * @param $param = 
 * @return 팝업 html
 */
function makeEstiRegiContHtml($param) {
    $html = <<<html
        <div id="esti_cont_ctrl">
    			<ul class="tab_box mt25">
    			    <li class="fl"><a class="box"> 견적내용 </a> </li>
    			</ul>
			    <form>
				<div class="tab_box_con">
				<fieldset>
                <legend> 문의내용 </legend>	
				<div class="form-group">
					<label class="control-label fix_width120 tar">문의제목</label><label class="fix_width20 fs14 tac">:</label>
					<label class="control-label cp">$param[title]</label>
					<br /><br />
					<label class="control-label fix_width120 tar">회원명</label><label class="fix_width20 fs14 tac">:</label>
					<label class="control-label cp">$param[office_nick]($param[member_name])</label>
					<br />
					<label class="control-label fix_width120 tar">수량</label><label class="fix_width20 fs14 tac">:</label>
					<label class="control-label cp">$param[amt]매 $param[count]건</label>
					<label class="fix_width40"></label>
					<label class="control-label fix_width120 tar">첨부파일</label><label class="fix_width20 fs14 tac">:</label>
                      $param[file_html]
                      <br />
					<label class="control-label fix_width120 tar">문의내용</label><label class="fix_width20 fs14 tac">:</label>
					<textarea class="bs_noti3 fix_width806" disabled>$param[inq_cont]</textarea>
					<br />
				</div>			     
				</fieldset>

				<fieldset class="mt25">
                <legend> 종이정보 </legend>		
				  $param[paper]
                <div id="paper_ctrl">
					<div class="form-group" id="paper_group1">
						<label class="control-label fix_width120 tar">종이</label><label class="fix_width20 fs14 tac">:</label>
						<select class="fix_width130" id="paper_name1" onchange="selectOptionVal.exec('paper', 'DVS', '1')">
                          $param[paper_option_html]
						</select>
						<select class="fix_width130" id="paper_dvs1" onchange="selectOptionVal.exec('paper', 'COLOR', '1')">
							<option value="">구분(전체)</option>
						</select>
						<select class="fix_width130" id="paper_color1" onchange="selectOptionVal.exec('paper', 'BASISWEIGHT', '1')">
							<option value="">색상(전체)</option>
						</select>
						<select class="fix_width130" id="paper_basisweight1">
							<option value="">평량(전체)</option>
						</select>
						<label class="fix_width10"></label>
						*
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width55" id="paper_amt1" value="1">
						<select class="fix_width55" id="paper_unit1">
							<option value="R">R</option>
							<option value="장">장</option>
						</select>
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width100" id="paper_price1">
						<button type="button" class="btn btn-sm btn-success" onclick="selectOptionVal.price('paper', '1')">계산</button>
						<button type="button" class="btn btn-sm btn-info paper_plus paper_plus1" onclick="addInfoGroup.exec('paper_ctrl', 'paper', 1);"><i class="fa fa-plus"></i></button>
						<button type="button" class="btn btn-sm btn-warning paper_minus" style="display:none;" onclick="delInfoGroup('paper_group1');"><i class="fa fa-minus"></i></button>
					</div>
                </div>
                </fieldset>
				
				<fieldset class="mt25">
                <legend> 출력정보 </legend>			
				  $param[size]
                  <div id="output_ctrl"> 
					<div class="form-group" id="output_group1">
						<label class="control-label fix_width120 tar">출력</label><label class="fix_width20 fs14 tac">:</label>
						<select class="fix_width220" id="output_name1" onchange="selectOptionVal.exec('output', 'BOARD', '1')">
                          $param[output_option_html]
						</select>
						<select class="fix_width180" id="output_board1" onchange="selectOptionVal.exec('output', 'SIZE', '1')">
							<option value="">출력판(전체)</option>
						</select>
						<select class="fix_width125" id="output_size1">
							<option value="">사이즈(전체)</option>
						</select>
						<label class="fix_width10"></label>
						*
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width55" value="1" id="output_amt1">
						<select class="fix_width55" id="output_unit1">
							<option value="판">판</option>
						</select>
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width100" id="output_price1">
						<button type="button" class="btn btn-sm btn-success" onclick="selectOptionVal.price('output', '1')">계산</button>
						<button type="button" class="btn btn-sm btn-info output_plus output_plus1" onclick="addInfoGroup.exec('output_ctrl', 'output', 1);"><i class="fa fa-plus"></i></button>
						<button type="button" class="btn btn-sm btn-warning output_minus" style="display:none;" onclick="delInfoGroup('output_group1');"><i class="fa fa-minus"></i></button>
					</div>
				</div>
				</fieldset>
				
				<fieldset class="mt25">
                <legend> 인쇄정보 </legend>			
				  $param[print_tmpt]
                  <div id="print_ctrl"> 
					<div class="form-group" id="print_group1">
						<label class="control-label fix_width120 tar">인쇄</label><label class="fix_width20 fs14 tac">:</label>
						<select class="fix_width180" id="cate_sortcode1" onchange="selectOptionVal.exec('print', 'NAME', '1')">
                          $param[print_option_html]
						</select>
						<select class="fix_width220" id="print_name1" onchange="selectOptionVal.exec('print', 'PURP', '1')">
							<option value="">인쇄명(전체)</option>
						</select>
						<select class="fix_width125" id="print_purp1">
							<option value="">용도구분(전체)</option>
						</select>        		    
						<label class="fix_width10"></label>
						*
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width55" value="1" id="print_amt1">
						<select class="fix_width55" id="print_unit1">
							<option value="R">R</option>
							<option value="장">장</option>
						</select>
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width100" id="print_price1">
						<button type="button" class="btn btn-sm btn-success" onclick="selectOptionVal.price('print', '1')">계산</button>
						<button type="button" class="btn btn-sm btn-info print_plus print_plus1" onclick="addInfoGroup.exec('print_ctrl', 'print', 1);"><i class="fa fa-plus"></i></button>
						<button type="button" class="btn btn-sm btn-warning print_minus" style="display:none;" onclick="delInfoGroup('print_group1');"><i class="fa fa-minus"></i></button>
					</div>
				</div>
				</fieldset>
				
				<fieldset class="mt25">
                <legend> 후공정정보 </legend>
				    $param[after]
                  <div id="after_ctrl"> 
					<div class="form-group" id="after_group1">
						<label class="control-label fix_width120 tar">후공정</label><label class="fix_width20 fs14 tac">:</label>
						<select class="fix_width130" id="after_name1" onchange="selectOptionVal.exec('after', 'DEPTH1', '1')">
                          $param[after_option_html]
						</select>
						<select class="fix_width130" id="after_depth_one1" onchange="selectOptionVal.exec('after', 'DEPTH2', '1')">
							<option value="">Depth1(전체)</option>
						</select>
						<select class="fix_width130" id="after_depth_two1" onchange="selectOptionVal.exec('after', 'DEPTH3', '1')">
							<option value="">Depth2(전체)</option>
						</select>
						<select class="fix_width130" id="after_depth_thr1">
							<option value="">Depth3(전체)</option>
						</select>
						<label class="fix_width10"></label>
						*
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width55" value="1" id="after_amt1">
						<select class="fix_width55" id="after_unit1">
							<option value="R">R</option>
							<option value="장">장</option>
						</select>        		          		    
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width100" id="after_price1">
						<button type="button" class="btn btn-sm btn-success" onclick="selectOptionVal.price('after', '1')">계산</button>
						<button type="button" class="btn btn-sm btn-info after_plus after_plus1" onclick="addInfoGroup.exec('after_ctrl', 'after', 1);"><i class="fa fa-plus"></i></button>
						<button type="button" class="btn btn-sm btn-warning after_minus" style="display:none;" onclick="delInfoGroup('after_group1');"><i class="fa fa-minus"></i></button>
					</div>
				</div>
				</fieldset>
				
				<fieldset class="mt25">
                <legend> 기타 </legend>
                  <div id="etc_ctrl"> 
					<div class="form-group" id="etc_group1">
						<label class="control-label fix_width120 tar">기타</label><label class="fix_width20 fs14 tac">:</label>
						<input type="text" class="input_co2 fix_width697" id="etc_cont1">
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width100" id="etc_price1">
						<button type="button" class="btn btn-sm btn-info etc_plus etc_plus1" onclick="addInfoGroup.exec('etc_ctrl', 'etc', 1);"><i class="fa fa-plus"></i></button>
						<button type="button" class="btn btn-sm btn-warning etc_minus" style="display:none;" onclick="delInfoGroup('etc_group1');"><i class="fa fa-minus"></i></button>			        
					</div>          
				</div>          
				</fieldset>
				
				<fieldset class="mt25">
                <legend> 답변내용 </legend>
            		<ul class="form-group">
						<li class="fix_width760 fl">                            	  	  
							<label class="control-label fix_width120 tar">전달메모</label><label class="fix_width20 fs14 tac">:</label>
							<textarea class="bs_noti2 fix_width577 fix_height114" id="memo">$param[memo]</textarea>                                  	  	  
						</li>                                                          
						<li class="fix_width280 fl">
							<label class="control-label fix_width81 tar">공급가액</label><label class="fix_width20 fs14 tac">:</label>
							<input type="text" class="input_dis fix_width100" id="supply_price" value="$param[supply_price]" disabled="disabled">
						    <button type="button" class="btn btn-sm btn-success" onclick="getSupply()">계산</button>
							<br />
							<label class="control-label fix_width81 tar">부가가치세</label><label class="fix_width20 fs14 tac">:</label>
							<input type="text" class="input_dis fix_width100" id="vat" value="$param[vat]" disabled="disabled">
							<br />
							<label class="control-label fix_width81 tar">DC</label><label class="fix_width20 fs14 tac">:</label>
							<input type="text" class="input_co2 fix_width100" id="sale_price" onkeyup="getEstimated(event, this);" value="$param[sale_price]">
							<br />
							<label class="control-label fix_width81 tar">견적가액</label><label class="fix_width20 fs14 tac">:</label>
							<input type="text" class="input_dis fix_width100" id="esti_price" value="$param[esti_price]" disabled="disabled">     
							<br />                               
						</li>                                          
					</ul>  
					<br /><br /><br /><br />
					<ul class="form-group">
						<br /><br /><br /><br /><br />
						<label class="control-label fix_width120 tar">답변내용</label><label class="fix_width20 fs14 tac">:</label>
						<textarea class="bs_noti3 fix_width806" id="answ_cont">$param[answ_cont]</textarea>
					</ul>  
									  
					<ul class="form-group">
						<br /><br />
						
						<label class="control-label fix_width120 tar">견적파일첨부</label><label class="fix_width20 fs14 tac">:</label>
						<!-- 파일 업로드 / 스크립이 하단에 있어야 작동함 -->                    
						<input id="upload_path" class="disableInputField" placeholder="Choose File" disabled="disabled" />
						
						<label class="fileUpload">
							<input id="upload_file" type="file" class="upload" onchange="fileSearchBtn(this.value);"/>
							<span class="btn btn-sm btn-info fa">파일찾기</span>
						</label>
                        $param[admin_file_html]
					</ul>			
					<ul class="form-group">
						<label class="control-label fix_width120 tar">예상주문일</label><label class="fix_width20 fs14 tac">:</label>
						<input type="text" class="input_co2 fix_width201" id="expec_order_date" placeholder="yyyy-MM-dd" value="$param[expec_order_date]">
					</ul>  

				</fieldset>
				<br />
			    <p class="btn-lg red_btn">
                    <a style="cursor: pointer;" onclick="regiEsti('$param[esti_seqno]');">등록한 조건으로 견적</a>					
                </p>
				</div>	
				</div>			     
			    </form>
                </div>
html;

    return $html;
}

/**
 * @brief 견적 등록 정보 종이 로우 html 생성
 * @param $param = 
 * @return html
 */
function makePaperLowHtml($param) {

    $html = <<<html
					<div class="form-group" id="paper_group$param[idx]">
						<label class="control-label fix_width79 tar">종이</label><label class="fix_width20 fs14 tac">:</label>
						<select class="fix_width100" id="paper_name$param[idx]" onchange="selectOptionVal.exec('paper', 'DVS', '$param[idx]')">
                          $param[paper_option_html]
						</select>
						<select class="fix_width100" id="paper_dvs$param[idx]" onchange="selectOptionVal.exec('paper', 'COLOR', '$param[idx]')">
							<option value="">구분(전체)</option>
						</select>
						<select class="fix_width100" id="paper_color$param[idx]" onchange="selectOptionVal.exec('paper', 'BASISWEIGHT', '$param[idx]')">
							<option value="">색상(전체)</option>
						</select>
						<select class="fix_width100" id="paper_basisweight$param[idx]">
							<option value="">평량(전체)</option>
						</select>
						<label class="fix_width10"></label>
						*
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width55" id="paper_amt$param[idx]" value="1">
						<select class="fix_width55" id="paper_unit$param[idx]">
							<option value="R">R</option>
							<option value="장">장</option>
						</select>
						<label class="fix_width10"></label>
						<input type="text" class="input_dis fix_width100" id="paper_price$param[idx]" disabled="disabled">
						<button type="button" class="btn btn-sm btn-success" onclick="selectOptionVal.price('paper', '$param[idx]')">계산</button>
						<button type="button" class="btn btn-sm btn-info paper_plus paper_plus$param[idx]" onclick="addInfoGroup.exec('paper_ctrl', 'paper', '$param[idx]');"><i class="fa fa-plus"></i></button>
						<button type="button" class="btn btn-sm btn-warning paper_minus" style="display:none;" onclick="delInfoGroup('paper_group$param[idx]');"><i class="fa fa-minus"></i></button>

html;

    return $html;
}

/**
 * @brief 견적 등록 정보 출력 로우 html 생성
 * @param $param = 
 * @return html
 */
function makeOutputLowHtml($param) {

    $html = <<<html
					<div class="form-group" id="output_group$param[idx]">
						<label class="control-label fix_width79 tar">출력</label><label class="fix_width20 fs14 tac">:</label>
						<select class="fix_width130" id="output_name$param[idx]" onchange="selectOptionVal.exec('output', 'BOARD', '$param[idx]')">
                          $param[output_option_html]
						</select>
						<select class="fix_width150" id="output_board$param[idx]" onchange="selectOptionVal.exec('output', 'SIZE', '$param[idx]')">
							<option value="">출력판(전체)</option>
						</select>
						<select class="fix_width120" id="output_size$param[idx]">
							<option value="">사이즈(전체)</option>
						</select>
						<label class="fix_width10"></label>
						*
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width55" value="1" id="output_amt$param[idx]">
						<select class="fix_width55" id="output_unit$param[idx]">
							<option value="판">판</option>
						</select>
						<label class="fix_width10"></label>
						<input type="text" class="input_dis fix_width100" id="output_price$param[idx]" disabled="disabled">
						<button type="button" class="btn btn-sm btn-success" onclick="selectOptionVal.price('output', '$param[idx]')">계산</button>
						<button type="button" class="btn btn-sm btn-info output_plus output_plus1" onclick="addInfoGroup.exec('output_ctrl', 'output', '$param[idx]');"><i class="fa fa-plus"></i></button>
						<button type="button" class="btn btn-sm btn-warning output_minus" style="display:none;" onclick="delInfoGroup('output_group$param[idx]');"><i class="fa fa-minus"></i></button>

html;

    return $html;
}

/**
 * @brief 견적 등록 정보 인쇄 로우 html 생성
 * @param $param = 
 * @return html
 */
function makePrintLowHtml($param) {

    $html = <<<html
					<div class="form-group" id="print_group$param[idx]">
						<label class="control-label fix_width79 tar">인쇄</label><label class="fix_width20 fs14 tac">:</label>
						<select class="fix_width150" id="cate_sortcode$param[idx]" onchange="selectOptionVal.exec('print', 'NAME', '$param[idx]')">
                          $param[print_option_html]
						</select>
						<select class="fix_width125" id="print_name$param[idx]" onchange="selectOptionVal.exec('print', 'PURP', '$param[idx]')">
							<option value="">인쇄명(전체)</option>
						</select>
						<select class="fix_width125" id="print_purp$param[idx]">
							<option value="">용도구분(전체)</option>
						</select>        		    
						<label class="fix_width10"></label>
						*
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width55" value="1" id="print_amt$param[idx]">
						<select class="fix_width55" id="print_unit$param[idx]">
							<option value="R">R</option>
							<option value="장">장</option>
						</select>
						<label class="fix_width10"></label>
						<input type="text" class="input_dis fix_width100" id="print_price$param[idx]" disabled="disabled">
						<button type="button" class="btn btn-sm btn-success" onclick="selectOptionVal.price('print', '$param[idx]')">계산</button>
						<button type="button" class="btn btn-sm btn-info print_plus print_plus$param[idx]" onclick="addInfoGroup.exec('print_ctrl', 'print', $param[idx]);"><i class="fa fa-plus"></i></button>
						<button type="button" class="btn btn-sm btn-warning print_minus" style="display:none;" onclick="delInfoGroup('print_group$param[idx]');"><i class="fa fa-minus"></i></button>
					</div>

html;

    return $html;
}

/**
 * @brief 견적 등록 정보 후공정 로우 html 생성
 * @param $param = 
 * @return html
 */
function makeAfterLowHtml($param) {

    $html = <<<html
					<div class="form-group" id="after_group$param[idx]">
						<label class="control-label fix_width79 tar">후공정</label><label class="fix_width20 fs14 tac">:</label>
						<select class="fix_width100" id="after_name$param[idx]" onchange="selectOptionVal.exec('after', 'DEPTH1', '$param[idx]')">
                          $param[after_option_html]
						</select>
						<select class="fix_width100" id="after_depth_one$param[idx]" onchange="selectOptionVal.exec('after', 'DEPTH2', '$param[idx]')">
							<option value="">Depth1(전체)</option>
						</select>
						<select class="fix_width100" id="after_depth_two$param[idx]" onchange="selectOptionVal.exec('after', 'DEPTH3', '$param[idx]')">
							<option value="">Depth2(전체)</option>
						</select>
						<select class="fix_width100" id="after_depth_thr$param[idx]">
							<option value="">Depth3(전체)</option>
						</select>
						<label class="fix_width10"></label>
						*
						<label class="fix_width10"></label>
						<input type="text" class="input_co2 fix_width55" value="1" id="after_amt$param[idx]">
						<select class="fix_width55" id="after_unit$param[idx]">
							<option value="R">R</option>
							<option value="장">장</option>
						</select>        		          		    
						<label class="fix_width10"></label>
						<input type="text" class="input_dis fix_width100" id="after_price$param[idx]" disabled="disabled">
						<button type="button" class="btn btn-sm btn-success" onclick="selectOptionVal.price('after', '$param[idx]')">계산</button>
						<button type="button" class="btn btn-sm btn-info after_plus after_plus1" onclick="addInfoGroup.exec('after_ctrl', 'after', '$param[idx]');"><i class="fa fa-plus"></i></button>
						<button type="button" class="btn btn-sm btn-warning after_minus" style="display:none;" onclick="delInfoGroup('after_group$param[idx]');"><i class="fa fa-minus"></i></button>
					</div>

html;

    return $html;
}

/**
 * @brief 견적 등록 정보 기타 로우 html 생성
 * @param $param = 
 * @return html
 */
function makeEtcLowHtml($param) {

    $html = <<<html
					<div class="form-group" id="etc_group$param[idx]">
						<label class="control-label fix_width79 tar">기타</label><label class="fix_width20 fs14 tac">:</label>
						<input type="text" class="input_co2 fix_width550" id="etc_cont$param[idx]">
						<label class="fix_width37"></label>
						<input type="text" class="input_co2 fix_width100" id="etc_price$param[idx]" onkeyup="getEtcPrice(event, this);">
						<button type="button" class="btn btn-sm btn-info etc_plus etc_plus$param[idx]" onclick="addInfoGroup.exec('etc_ctrl', 'etc', '$param[idx]');"><i class="fa fa-plus"></i></button>
						<button type="button" class="btn btn-sm btn-warning etc_minus" style="display:none;" onclick="delInfoGroup('etc_group$param[idx]');"><i class="fa fa-minus"></i></button>			        
					</div>    
html;

    return $html;
}
?>
