<?
//종이설명 팝업 html
function getPaperDscrHtml($param) {

    $html = <<<VIEWHTML

    <form name="paper_form" id="paper_form" method="post">
 
                        			    <dl>
                        			        <dt class="tit">
                        			       	    <h4>종이 기본설명 관리</h4>
                        			        </dt>
                        			        <dt class="cls">
                        			       	    <button type="button" onclick="hideRegiPopup(); return false;" class="btn btn-sm btn-danger fa fa-times"></button>
                        			        </dt>
                        			    </dl>
                        														 	
                        			    <div class="pop-base">                                    
                                            <div class="pop-content">								 	  	
                                                <div class="form-group">                               		 
                                        		    <label class="control-label fix_width79 tar">대분류</label><label class="fix_width20 fs14 tac">:</label>
                        			    		    <input type="text" id="top" name="top" class="input_co2 fix_width180" value="$param[top]">
                        			    		    <br />		    

                                        		    <label class="control-label fix_width79 tar">종이명</label><label class="fix_width20 fs14 tac">:</label>
                        			    		    <input type="text" id="paper_name" name="paper_name" class="input_co2 fix_width180" value="$param[name]">
                        			    		    <br />		    
                        			    				         
                                                    <label class="control-label fix_width79 tar">구분</label><label class="fix_width20 fs14 tac">:</label>
                                                    <input type="text" class="input_co2 fix_width180" id="dvs" name="dvs" value="$param[dvs]">
                                                    <br />

                                                    <label class="control-label fix_width79 tar">색상</label><label class="fix_width20 fs14 tac">:</label>
                                                    <input type="text" class="input_co2 fix_width180" id="color" name="color" value="$param[color]">
                                                    <br />

                                                    <label class="control-label fix_width79 tar">평량</label><label class="fix_width20 fs14 tac">:</label>
                                                    <input type="text" class="input_co2 fix_width180" id="basisweight" name="basisweight" value="$param[basisweight]">
                                                    <br />

                                                    <label class="control-label fix_width79 tar">재질설명</label><label class="fix_width20 fs14 tac">:</label>
                                                    <input type="text" class="input_co2 fix_width180" id="dscr" name="dscr" value="$param[dscr]">
                                                    <br />

                                                    <label class="control-label fix_width79 tar">용도</label><label class="fix_width20 fs14 tac">:</label>
                                                    <input type="text" class="input_co2 fix_width180" id="purp" name="purp" value="$param[purp]">
                                                    <br />
                                                    
                                                    <label class="control-label fix_width79 tar">느낌</label><label class="fix_width20 fs14 tac">:</label>
                                                    <input type="text" class="input_co2 fix_width180" id="sense" name="sense" value="$param[sense]">
                                                    <br />

                                                    <label class="control-label fix_width79 tar">후공정표</label><label class="fix_width20 fs14 tac">:</label>
                                                    <input type="text" class="input_co2 fix_width180" id="able_after" name="able_after" value="$param[able_after]">
                                                    <br />

                                                    <label class="control-label fix_width79 tar">체크포인트</label><label class="fix_width20 fs14 tac">:</label>
                                                    <input type="text" class="input_co2 fix_width180" id="warn" name="warn" value="$param[warn]">
                                                    <br />
                                                </div>
                                                
                                                <hr class="hr_bd3_b">
                        			    		     
                                                <div class="form-group">
                                                    <p class="tac mt15">
                                                        <label></label>
                                            		    <button type="button" onclick="savePaperDscr('$param[paper_dscr_seqno]'); return false;" class="btn btn-sm btn-success">저장</button>
                                                        <button type="button" onclick="hideRegiPopup(); return false;" class="btn btn-sm btn-primary">닫기</button>
                                                        <button type="button" onclick="delPaperDscr('$param[paper_dscr_seqno]'); return false;" class="btn btn-sm btn-danger">삭제</button>
                                                    </p> 
                                                </div>                                                                                         
                        			        </div> 
                        			    </div>                    			    

    </form>
VIEWHTML;

    return $html;

}

//직배등록 팝업 html
function putDirectDlvrHtml($param) {

    $html = <<<VIEWHTML

    <form name="paper_form" id="paper_form" method="post">
 
                        			    <dl>
                        			        <dt class="tit">
                        			       	    <h4>차량등록 관리</h4>
                        			        </dt>
                        			        <dt class="cls">
                        			       	    <button type="button" onclick="hideRegiPopup(); return false;" class="btn btn-sm btn-danger fa fa-times"></button>
                        			        </dt>
                        			    </dl>
                        														 	
                        			    <div class="pop-base">                                    
                                            <div class="pop-content">								 	  	
                                                <div class="form-group">                               		 
                                        		    <label class="control-label fix_width79 tar">차량이름</label><label class="fix_width20 fs14 tac">:</label>
                        			    		    <input type="text" id="vehi_num" name="vehi_num" class="input_co2 fix_width180" value="">
                        			    		    <br />		    

                                        		    <label class="control-label fix_width79 tar">담당자명</label><label class="fix_width20 fs14 tac">:</label>
                        			    		    <input type="text" id="mng" name="mng" class="input_co2 fix_width180" value="">
                        			    		    <br />		    
                        			    				         
                                                    <label class="control-label fix_width79 tar">배송지역</label><label class="fix_width20 fs14 tac">:</label>
                                                    <input type="text" class="input_co2 fix_width180" id="dlvr_area" name="dlvr_area" value="">
                                                    <br />

                                                    <label class="control-label fix_width79 tar">차량번호</label><label class="fix_width20 fs14 tac">:</label>
                                                    <input type="text" class="input_co2 fix_width180" id="car_number" name="car_number" value="">
                                                    <br />
                                                </div>
                                                
                                                <hr class="hr_bd3_b">
                        			    		     
                                                <div class="form-group">
                                                    <p class="tac mt15">
                                                        <label></label>
                                            		    <button type="button" onclick="saveDirectCar(); return false;" class="btn btn-sm btn-primary">저장</button>
                                                        <button type="button" onclick="hideRegiPopup(); return false;" class="btn btn-sm btn-success">닫기</button>
                                                    </p> 
                                                </div>                                                                                         
                        			        </div> 
                        			    </div>                    			    

    </form>
VIEWHTML;

    return $html;

}

//후공정설명 팝업 html
function getAfterDscrHtml($param) {

    $html = <<<VIEWHTML

    <form name="after_form" id="after_form" method="post">

                        			    <dl>
                        			        <dt class="tit">
                        			       	    <h4>후공정 기본설명 관리</h4>
                        			        </dt>
                        			        <dt class="cls">
                        			       	    <button type="button" onclick="hideRegiPopup(); return false;" class="btn btn-sm btn-danger fa fa-times"></button>
                        			        </dt>
                        			    </dl>
                        			    <div class="pop-base">
                                            <div class="pop-content">
                                                <div class="form-group">
                                        		    <label class="control-label fix_width79 tar">후공정명</label><label class="fix_width20 fs14 tac">:</label>
                        			    		    <input type="text" class="input_co2 fix_width250" id="after_name" name="after_name" value="$param[name]">
                        			    		    <br />		    
                        			    				         
                                                    <label class="control-label fix_width79 tar">후공정설명</label><label class="fix_width20 fs14 tac">:</label>
                                                    <textarea class="bs_noti2" name="dscr" id="after_dscr">$param[dscr]</textarea>
                                                    <br />                                               
                                                </div>
                                                
                                                <hr class="hr_bd3_b">
                        			    		     
                                                <div class="form-group">
                                                    <p class="tac mt15">
                                            		    <button type="button" onclick="saveAfterDscr('$param[after_dscr_seqno]'); return false;" class="btn btn-sm btn-success">저장</button>
                                                        <button type="button" onclick="hideRegiPopup(); return false;" class="btn btn-sm btn-primary">닫기</button>
                                                        <button type="button" onclick="delAfterDscr('$param[after_dscr_seqno]'); return false;" class="btn btn-sm btn-danger">삭제</button>
                                                    </p>
                                                </div>
                        			        </div>
                        			    </div>

    </form>
VIEWHTML;

    return $html;

}

//옵션설명 팝업 html
function getOptDscrHtml($param) {

    $html = <<<VIEWHTML

    <form name="opt_form" id="opt_form" method="post">

                                
                        			    <dl>
                        			        <dt class="tit">
                        			       	    <h4>옵션 기본설명 관리</h4>
                        			        </dt>
                        			        <dt class="cls">
                        			       	    <button type="button" onclick="hideRegiPopup(); return false;" class="btn btn-sm btn-danger fa fa-times"></button>
                        			        </dt>
                        			    </dl>
                        			    <div class="pop-base">
                                            <div class="pop-content">
                                                <div class="form-group">
                                        		    <label class="control-label fix_width79 tar">옵션명</label><label class="fix_width20 fs14 tac">:</label>
                        			    		    <input type="text" class="input_co2 fix_width250" name="opt_name" id="opt_name" value="$param[name]">
                        			    		    <br />		    
                        			    				         
                                                    <label class="control-label fix_width79 tar">옵션설명</label><label class="fix_width20 fs14 tac">:</label>
                                                    <textarea name="dscr" id="opt_dscr" class="bs_noti2">$param[dscr]</textarea>
                                                    <br />                                               
                                                </div>
                                                <hr class="hr_bd3_b">
                        			    		     
                                                <div class="form-group">
                                                    <p class="tac mt15">
                                            		    <button onclick="saveOptDscr('$param[opt_dscr_seqno]'); return false;" type="button" class="btn btn-sm btn-success">저장</button>
                                                        <button onclick="hideRegiPopup(); return false;" type="button" class="btn btn-sm btn-primary">닫기</button>
                                                        <button onclick="delOptDscr('$param[opt_dscr_seqno]'); return false;" type="button" class="btn btn-sm btn-danger">삭제</button>
                                                    </p>
                                                </div>
                        			        </div>
                        			    </div>
 
    </form>
VIEWHTML;

    return $html;

}




?>
