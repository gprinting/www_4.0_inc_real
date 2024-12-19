<?
function getPurEtprsRegi($param1, $param2) {
    $date = date("Y-m-d", time());
    $html = <<<HTML
														 	<dl>
														 	    <dt class="tit">
														 	    	  <h4>상담내역</h4>
														 	    </dt>
														 	    <dt class="cls">
														 	    	  <button type="button" class="btn btn-sm btn-danger fa fa-times" onclick="hideRegiPopup();"></button>
														 	    </dt>
														 	</dl>
														 	 
														 	 <div class="pop-base">
							                    	           <div class="pop-content">
                                                               <div class="tab-content">
                                                               <form name="regi_form" id="regi_form">
							                    	           <div class="form-group">
                                                               <label class="fix_width28"> </label>
							                    	               <label class="control-label fix_width120 ">작성자</label><label class="fix_width20 fs14 tac">:</label>
							                    	               <input type="text" class="input_co2 " name="con_name" maxlength="30" value="$param1">
                                                                   <input type="hidden" class="input_co2 " name="con_custom" maxlength="30" value="$param2">
                                                                   <br />
							                    	               <label class="fix_width28"> </label>
							                    	               <label class="control-label fix_width120 ">상담일</label><label class="fix_width20 fs14 tac">:</label>
                                                                     <input type="text" class="input_co2 " name="con_date" maxlength="30" value="$date">
                                                                   <br /><br />
							                    	               <label class="fix_width28"> </label>
							                    	               <textarea class="" name="con_msg" value="" style="width:600px;"> </textarea>
                                                                   </div>

                                                               <hr class="hr_bd3">
                                                               <div style="text-align:center;">
                                                                   <label class="btn btn_md fix_width140 fr" onclick="regicontext();"><i class="fa fa-check-square"></i> 상담이력저장하기</label>
							                    	           </div>
                                                               </form>
                                                               </div>
                                                        </div>
													</div>

                       	      </div>  
                       	      <!-- Regi 팝업창 -->
	
HTML;

    return $html;
}


function makeOeventList($result) {

    $ret = "";

    $i = 1;
														
    $list2 = "\n <dl>";
	$list2 .="\n <dt class='tit'>";
	$list2 .="\n <h4>상담내역</h4>";
	$list2 .="\n </dt>";
	$list2 .="\n <dt class='cls'>";
	$list2 .="\n	 <button type='button' class='btn btn-sm btn-danger fa fa-times' onclick='hideRegiPopup();'></button>";
	$list2 .="\n	 </dt>";
	$list2 .="\n	</dl> ";

    $list2 .="\n	<div class='div_table_container'><table class='table_member_common_list'><thead><tr>";
	$list2 .="\n <th class='bm2px'>No</th>";
	$list2 .="\n <th class='bm2px'>상담날짜</th>";
	$list2 .="\n <th class='bm2px'>회원명</th>";
    $list2 .="\n <th class='bm2px'>상담자</th>";
    $list2 .="\n <th class='bm2px'>상담내용</th></tr></thead><tbody id='member_list'>";


     if ($i%2 == 1) {
        $list  .= "\n  <tr>";
    } else {
       $list  .= "\n  <tr class=\"cellbg\">";
    }
    $list .= "\n    <td>%s</td>";
    $list .= "\n    <td>%s</td>";
    $list .= "\n    <td>%s</td>";
    $list .= "\n    <td>%s</td>";
    $list .= "\n    <td>%s</td>";
    $list .= "\n  </tr>";										
															
														

    while ($result && !$result->EOF) {

        $con_idx = $result->fields["con_idx"];
        $con_man = $result->fields["con_man"];
        $con_custom = $result->fields["con_custom"];
        $con_date = $result->fields["con_date"];
        $con_text = $result->fields["con_text"];



        if ($dsply_yn == "Y") {

            $dsply_str = "진열";

        } else {

            $dsply_str = "미진열";

        }

        $cate_name = $result->fields["cate_name"];
        $sell_site = $result->fields["sell_site"];
        $oevent_seqno = $result->fields["oevent_event_seqno"];

       

        $ret .= sprintf($list, $con_idx, $con_date,  $con_custom, $con_man,$con_text); 

        $result->moveNext();
        $i++; 
    }

    return $list2.$ret;
}


?>
