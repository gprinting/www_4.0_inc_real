<?
/**
 * @brief 등록 팝업 공통 HTML
 */
function commonRegiPopupHtml($title) {
 
    $html  = "\n<dl>";
    $html .= "\n   <dt class=\"tit\">";
    $html .= "\n        <h4>" . $title . "</h4>";
    $html .= "\n   </dt>";
    $html .= "\n   <dt class=\"cls\">";
    $html .= "\n        <button type=\"button\" class=\"btn btn-sm btn-danger fa fa-times\" onclick=\"hideRegiPopup();\"></button>";
    $html .= "\n   </dt>";
    $html .= "\n</dl>";

    return $html;
}

/**
 * @brief 등록 팝업 공통 버튼 HTML
 */
function commonRegiPopupBtnHtml($el, $flag, $seqno) {
 
    $html  = "\n       <hr class=\"hr_bd3\">";
    $html .= "\n       <p class=\"tac mt15\">";
    $html .= "\n          <button type=\"button\" class=\"btn btn-sm btn-success\" onclick=\"insertInfo('" . $el . "', '" . $seqno . "');\">저장</button>";

    if ($flag) { 
        $html .= "\n          <label class=\"fix_width5\"> </label>";
        $html .= "\n          <button type=\"button\" id=\"delete_btn\" class=\"btn btn-sm btn-danger\" onclick=\"deleteInfo('" . $el . "', '" . $seqno . "');\">삭제</button>";
    }

    $html .= "\n          <label class=\"fix_width140\"> </label>";
    $html .= "\n          <button type=\"button\" class=\"btn btn-sm btn-primary\" onclick=\"hideRegiPopup();\">닫기</button>";

    if ($flag) { 
        $html .= "\n          <label class=\"fix_width5\"> </label>";
        $html .= "\n          <button type=\"button\" class=\"btn btn-sm btn-success\" style=\"position:relative; left:100px;\" onclick=\"insertInfo('" . $el . "', '', 'T');\">항목추가</button>";
    }

    $html .= "\n       </p>";

    return $html;
}

/**
 * @brief 상품종이등록 팝업 HTML
 */
function paperRegiPopupHtml($param, $val) {

    $tmp = explode("♪", $param);

    $html  = commonRegiPopupHtml("상품종이품목관리");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n       <div class=\"form-group\">";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">종이 대분류</label><label class=\"fix_width10 fs14 tac\">:</label>";                                               
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_paper_sort\">";
    $html .= $tmp[0];
    $html .= "\n           </select>";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">종이명</label><label class=\"fix_width10 fs14 tac\">:</label>";                                                
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_paper_name\" value=\"" . $val["name"] . "\" maxlength=\"20\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">구분</label><label class=\"fix_width10 fs14 tac\">:</label>";                                              
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_paper_dvs\" value=\"" . $val["dvs"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">색상</label><label class=\"fix_width10 fs14 tac\">:</label>";                                              
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_paper_color\" value=\"" . $val["color"] . "\" maxlength=\"20\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">평량</label><label class=\"fix_width10 fs14 tac\">:</label>";                                              
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width73\" placeholder=\"\" id=\"pop_paper_basisweight\" value=\"" . $val["basisweight"] . "\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <select class=\"fix_width57\" id=\"pop_paper_basisweight_unit\">";
    $html .= "\n               <option value=\"g\">g </option>";
    $html .= "\n               <option value=\"μ\">μ</option>";
    $html .= "\n               <option value=\"T\">T</option>";
    $html .= "\n           </select>";
    $html .= "\n           <label class=\"control-label fix_width75 tar\" style=\"width:76px;\">계열</label><label class=\"fix_width10 fs14 tac\">:</label>";                                              
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_paper_affil\">";
    $html .= "\n               <option value=\"46\">46 </option>";
    $html .= "\n               <option value=\"국\">국</option>";
    $html .= "\n               <option value=\"A3\">A3</option>";
    $html .= "\n               <option value=\"별\">별</option>";
    $html .= "\n           </select>";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">기준단위</label><label class=\"fix_width10 fs14 tac\">:</label>";                                               
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_paper_crtr_unit\">";
    $html .= "\n               <option value=\"장\">장 </option>";
    $html .= "\n               <option value=\"R\">R</option>";
    $html .= "\n           </select>";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">사이즈</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" value=\"" . $val["wid_size"] . "\" onkeyup=\"onlyNumber2(event);\" id=\"pop_paper_wid_size\" maxlength=\"13\"> * <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" value=\"" . $val["vert_size"] . "\" onkeyup=\"onlyNumber2(event);\" id=\"pop_paper_vert_size\" maxlength=\"13\">";
    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("paper", $tmp[1], $tmp[2]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 상품종이등록 팝업 HTML
 */
function reworkPopupHtml($param, $val) {

    $count_html = "";
    for($i = 1; $i <= $param["count"]; $i++) {
        if($i == $param["count"])
            $count_html .= "<option selected value=\"" . $i . "\">" . $i . " </option>";
        else
            $count_html .= "<option value=\"" . $i . "\">" . $i . " </option>";
    }
    $tmp = explode("♪", $val);

    $html  = commonRegiPopupHtml("재작업/주문 등록");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n       <h4 style=\"font-size: 13px;\">재작업 분류</h4><br>";
    $html .= "\n       <div class=\"form-group\" style='display: inline-flex'>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"접수오류\" class=\"fix_width15\" id=\"rework_name01\" /><label for='\"rework_name01\"'>접수오류</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"조판불량\" class=\"fix_width15\" id=\"rework_name02\" /><label for='\"rework_name02\"'>조판불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"출력불량\" class=\"fix_width15\" id=\"rework_name03\" /><label for='\"rework_name03\"'>출력불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"인쇄불량\" class=\"fix_width15\" id=\"rework_name04\" /><label for='\"rework_name04\"'>인쇄불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"택배파손\" class=\"fix_width15\" id=\"rework_name05\" /><label for='\"rework_name05\"'>택배파손</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"접지불량\" class=\"fix_width15\" id=\"rework_name06\" /><label for='\"rework_name06\"'>접지불량</label></div>";
    $html .= "\n          </div>";
    $html .= "\n       <div class=\"form-group\" style='display: inline-flex'>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"포장불량\" class=\"fix_width15\" id=\"rework_name07\" /><label for='\"rework_name07\"'>포장불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"출고분실\" class=\"fix_width15\" id=\"rework_name08\" /><label for='\"rework_name08\"'>출고분실</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"배송불량\" class=\"fix_width15\" id=\"rework_name09\" /><label for='\"rework_name09\"'>배송불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"택배분실\" class=\"fix_width15\" id=\"rework_name10\" /><label for='\"rework_name10\"'>택배분실</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"택배지연\" class=\"fix_width15\" id=\"rework_name11\" /><label for='\"rework_name11\"'>택배지연</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"재단불량\" class=\"fix_width15\" id=\"rework_name12\" /><label for='\"rework_name12\"'>재단불량</label></div>";
    $html .= "\n          </div>";
    $html .= "\n       <div class=\"form-group\" style='display: inline-flex'>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"오시불량\" class=\"fix_width15\" id=\"rework_name13\" /><label for='\"rework_name13\"'>오시불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"제본불량\" class=\"fix_width15\" id=\"rework_name14\" /><label for='\"rework_name14\"'>제본불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"도무송불량\" class=\"fix_width15\" id=\"rework_name15\" /><label for='\"rework_name15\"'>도무송불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"형압불량\" class=\"fix_width15\" id=\"rework_name16\" /><label for='\"rework_name16\"'>형압불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"원단불량\" class=\"fix_width15\" id=\"rework_name17\" /><label for='\"rework_name17\"'>원단불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"귀도리불량\" class=\"fix_width15\" id=\"rework_name18\" /><label for='\"rework_name18\"'>귀도리불량</label></div>";
    $html .= "\n          </div>";
    $html .= "\n       <div class=\"form-group\" style='display: inline-flex'>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"미싱불량\" class=\"fix_width15\" id=\"rework_name19\" /><label for='\"rework_name19\"'>미싱불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"타공불량\" class=\"fix_width15\" id=\"rework_name20\" /><label for='\"rework_name20\"'>타공불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"넘버링불량\" class=\"fix_width15\" id=\"rework_name21\" /><label for='\"rework_name21\"'>넘버링불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"코팅불량\" class=\"fix_width15\" id=\"rework_name22\" /><label for='\"rework_name22\"'>코팅불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"목형불량\" class=\"fix_width15\" id=\"rework_name23\" /><label for='\"rework_name23\"'>목형불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"봉투가공불량\" class=\"fix_width15\" id=\"rework_name24\" /><label for='\"rework_name24\"'>봉투가공불량</label></div>";
    $html .= "\n          </div>";
    $html .= "\n       <div class=\"form-group\" style='display: inline-flex'>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"창가공불량\" class=\"fix_width15\" id=\"rework_name25\" /><label for='\"rework_name25\"'>창가공불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"양면테잎불량\" class=\"fix_width15\" id=\"rework_name26\" /><label for='\"rework_name26\"'>양면테잎불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"박불량\" class=\"fix_width15\" id=\"rework_name27\" /><label for='\"rework_name27\"'>박불량</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"송장부착오류\" class=\"fix_width15\" id=\"rework_name28\" /><label for='\"rework_name28\"'>송장부착오류</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"립에러\" class=\"fix_width15\" id=\"rework_name29\" /><label for='\"rework_name29\"'>립에러</label></div>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"전산오류\" class=\"fix_width15\" id=\"rework_name30\" /><label for='\"rework_name30\"'>전산오류</label></div>";
    $html .= "\n          </div>";
    $html .= "\n       <div class=\"form-group\" style='display: inline-flex'>";
    $html .= "\n              <div style='width: 140px;'><input type=\"radio\" name=\"type_rework\" value=\"접수조판시스템오류\" class=\"fix_width15\" id=\"rework_name31\" /><label for='\"rework_name31\"'>접수조판시스템오류</label></div>";
    $html .= "\n          </div><br><br>";
    $html .= "\n       <div class=\"form-group\" style='display: inline-flex'>";
    $html .= "\n              <div style='width: 100px;'><input type=\"radio\" name=\"type_rework\" value=\"재주문\" class=\"fix_width15\" id=\"rework_name31\" /><label for='\"rework_name31\"'>재주문</label></div>";
    $html .= "\n          </div><br><br><br><br>";
    $html .= "\n       <h4 style=\"font-size: 13px;\">세부사항</h4><br>";
    $html .= "\n           </select>";
    $html .= "\n           <table class='table_basic_layout_detail'>";
    $html .= "\n              <colgroup>";
    $html .= "\n                  <col width='230px'>";
    $html .= "\n                  <col width='230px'>";
    $html .= "\n                  <col width='230px'>";
    $html .= "\n              </colgroup>";
    $html .= "\n              <tbody>";
    $html .= "\n                  <tr>";
    $html .= "\n                      <th>회사 부담 비율</th>";
    $html .= "\n                      <th>고객 부담 비율</th>";
    $html .= "\n                      <th>회사 부담 금액</th>";
    $html .= "\n                  </tr>";
    $html .= "\n                  <tr>";
    $html .= "\n                      <td>";
    $html .= "\n                          <input type=\"radio\" name=\"type_ratio\" value=\"ratio_0\" class=\"fix_width15\" id=\"ratio0\" /><label for='\"ratio0\"'>0%</label>";
    $html .= "\n                          <input type=\"radio\" name=\"type_ratio\" value=\"ratio_25\" class=\"fix_width15\" id=\"ratio25\" /><label for='\"ratio25\"'>25%</label>";
    $html .= "\n                          <input type=\"radio\" name=\"type_ratio\" value=\"ratio_50\" class=\"fix_width15\" id=\"ratio50\" /><label for='\"ratio50\"'>50%</label>";
    $html .= "\n                          <input type=\"radio\" name=\"type_ratio\" value=\"ratio_75\" class=\"fix_width15\" id=\"ratio75\" /><label for='\"ratio75\"'>75%</label>";
    $html .= "\n                          <input type=\"radio\" checked name=\"type_ratio\" value=\"ratio_100\" class=\"fix_width15\" id=\"ratio100\" /><label for='\"ratio100\"'>100%</label>";
    $html .= "\n                      </td>";
    $html .= "\n                      <td>";
    $html .= "\n                          <label class=\"control-label fix_width75 tar\">- 공급가 : </label>";
    $html .= "\n                          <input type=\"text\" class=\"input_co2 fix_width140\" value='0' placeholder=\"\" id=\"pop_paper_name\" />";
    $html .= "\n                      </td>";
    $html .= "\n                      <td>";
    $html .= "\n                          <label class=\"control-label fix_width75 tar\">- 공급가 : </label>";
    $html .= "\n                          <input type=\"text\" class=\"input_co2 fix_width140\" value='".$param['price']."' placeholder=\"\" id=\"rework_price\" />";
    $html .= "\n                      </td>";
    $html .= "\n                  </tr>";
    $html .= "\n                  <tr>";
    $html .= "\n                      <td><label class=\"control-label fix_width75 tar\">- 건수 : </label>";
    $html .= "\n                      <select class=\"fix_width57\" id=\"rework_count\">";
    $html .= $count_html;
    $html .= "\n                      </select></td>";
    $html .= "\n                      <td>";
    $html .= "\n                          <label class=\"control-label fix_width75 tar\">- 부가세 : </label>";
    $html .= "\n                          <input type=\"text\" class=\"input_co2 fix_width140\" value='0' placeholder=\"\" id=\"pop_paper_name\" />";
    $html .= "\n                      </td>";
    $html .= "\n                  </tr>";
    $html .= "\n                  <tr>";
    $html .= "\n                      <td><label class=\"control-label fix_width75 tar\">- 매수 : </label>";
    $html .= "\n                      <input type=\"text\" class=\"input_co2 fix_width140\" value='".$param['amt']."' placeholder=\"\" id=\"rework_amt\" /></td>";
    $html .= "\n                      <td>";
    $html .= "\n                          <label class=\"control-label fix_width75 tar\">- 총합계 : </label>";
    $html .= "\n                          <input type=\"text\" class=\"input_co2 fix_width140\" value='0' placeholder=\"\" id=\"pop_paper_name\" />";
    $html .= "\n                      </td>";
    $html .= "\n                  </tr>";
    $html .= "\n              </tbody>";
    $html .= "\n           </table>";
    $html .= "\n           <br />";
    $html .= "\n           <div><label class=\"control-label fix_width75 tar\">재작업 사유</label><label class=\"fix_width10 fs14 tac\"> : </label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width570\" value='' placeholder=\"\" id=\"rework_cause_detail\" /></div>";
    $html .= "\n           <br />";
    $html .= "\n           <div><label class=\"control-label fix_width75 tar\">재작업 요청자</label><label class=\"fix_width10 fs14 tac\"> : </label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width570\" value='' placeholder=\"\" id=\"rework_request_empl\" /></div>";
    $html .= "\n           <br />";
    $html .= "\n           <div><label class=\"control-label fix_width75 tar\">등록자</label><label class=\"fix_width10 fs14 tac\"> : </label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width150\" value='황재호' placeholder=\"\" id=\"rework_id\" /></div>";
    $html .= "\n           <br />";
    $html .= "\n           <input type=\"checkbox\" name=\"type_ratio\" value=\"ratio_0\" class=\"fix_width15\" id=\"ratio0\" /><label for='\"ratio0\"'>고객노출여부</label>";
    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("rework", "", $val["seq"]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 상품종이등록 팝업 HTML
 */
function changePricePopupHtml($param, $val) {

    $count_html = "";
    for($i = 1; $i <= $param["count"]; $i++) {
        if($i == $param["count"])
            $count_html .= "<option selected value=\"" . $i . "\">" . $i . " </option>";
        else
            $count_html .= "<option value=\"" . $i . "\">" . $i . " </option>";
    }
    $tmp = explode("♪", $val);

    $html  = commonRegiPopupHtml("금액 수정");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n           <div style='text-align: center;'><label class=\"control-label fix_width75 tar\">총액</label><label class=\"fix_width10 fs14 tac\"> : </label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width200\" value='" . $val['price'] ."' placeholder=\"\" id=\"price\" style='width: 170px;'/></div>";
    $html .= "\n           <div style='margin-top:20px;text-align: center;'><label class=\"control-label fix_width75 tar\">변경 사유</label><label class=\"fix_width10 fs14 tac\"> : </label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width200\" value='' placeholder=\"\" id=\"change_cause_detail\" /></div>";
    $html .= "\n           <br />";
    $html .= commonRegiPopupBtnHtml("price", "", $val["seq"]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}


/**
 * @brief 상품종이등록 팝업 HTML
 */
function changeDlvrPopupHtml($param, $val) {

    $count_html = "";
    for($i = 1; $i <= $param["count"]; $i++) {
        if($i == $param["count"])
            $count_html .= "<option selected value=\"" . $i . "\">" . $i . " </option>";
        else
            $count_html .= "<option value=\"" . $i . "\">" . $i . " </option>";
    }
    $tmp = explode("♪", $val);

    $html  = commonRegiPopupHtml("송장번호 입력/수정");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n           <div style='text-align: center;'><label class=\"control-label fix_width75 tar\">택배사</label><label class=\"fix_width10 fs14 tac\"> : </label>";
    $html .= "\n           <select class=\"input_co2 fix_width200\" id=\"invo_kind\"><option>롯데택배</option><option>로젠택배</option><option>CJ택배</option><option>한진택배</option></select></div>";
    $html .= "\n           <div style='margin-top:20px;text-align: center;'><label class=\"control-label fix_width75 tar\">송장번호</label><label class=\"fix_width10 fs14 tac\"> : </label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width200\" value='" . $val['invo_num'] ."' placeholder=\"\" id=\"change_invo_num\" /></div>";
    $html .= "\n           <div style=\"margin-top:20px;text-align: center;\"><input type=\"radio\" name=\"bun_group\" class=\"input_co2\" value='1' placeholder=\"\" checked />묶음풀고 등록";
    $html .= "\n           <input type=\"radio\" name=\"bun_group\" class=\"input_co2\" value='2' placeholder=\"\" />묶음전부 등록</div>";
    $html .= "\n           <br />";
    $html .= commonRegiPopupBtnHtml("invo_num", "", $val["seq"]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 상품종이등록 팝업 HTML
 */
function loadSelectExtnlPopupHtml() {
    $html  = commonRegiPopupHtml("외주 작업지시서 양식");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n           <div style='text-align: center;'><label class=\"control-label fix_width75 tar\">양식</label><label class=\"fix_width10 fs14 tac\"> : </label>";
    $html .= "\n           <select class=\"input_co2 fix_width200\" id=\"extnl_kind\"><option value=\"nc1\">코팅/무코팅명함</option><option value=\"nc2\">수입지명함</option><option value=\"art90\">90아트전단</option><option value=\"card\">카드명함</option><option value=\"greenbag\">그린백</option><option value=\"banner\">배너</option><option value=\"magnet\">자석상품</option><option value=\"master\">마스타</option><option value=\"ametal\">A형철재물</option><option value=\"sticker\">사각재단스티커</option></select></div>";
    $html .= "\n           <br />";
    $html .= commonRegiPopupBtnHtml("select_extnl", "","");
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 상품종이등록 팝업 HTML
 */
function orderHistoryPopupHtml($param, $val, $tbody_form) {
    $count_html = "";
    for($i = 1; $i <= $param["count"]; $i++) {
        if($i == $param["count"])
            $count_html .= "<option selected value=\"" . $i . "\">" . $i . " </option>";
        else
            $count_html .= "<option value=\"" . $i . "\">" . $i . " </option>";
    }
    $tmp = explode("♪", $val);

    $html  = commonRegiPopupHtml("주문이력보기");
    $html .= "\n<table class=\"table_basic_layout\">";
    $html .= "\n                    <colgroup>";
    $html .= "\n                        <col style=\"width:100px;\">";
    $html .= "\n                        <col style=\"width:80px;\">";
    $html .= "\n                        <col style=\"width:330px;\">";
    $html .= "\n                        <col style=\"width:330px;\">";
    $html .= "\n                        <col style=\"width:80px;\">";
    $html .= "\n                    </colgroup>";
    $html .= "\n                    <thead>";
    $html .= "\n                        <th>등록일시</th>";
    $html .= "\n                        <th>종류</th>";
    $html .= "\n                        <th>변경전</th>";
    $html .= "\n                        <th>변경후</th>";
    $html .= "\n                        <th>담당자</th>";
    $html .= "\n                    </thead>";
    $html .= "\n                    <tbody id=\"sales_depo_list\">";
    $html .= $tbody_form;
    $html .= "\n                    </tbody>";
    $html .= "\n                </table>";
    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("rework", "", $val["seq"]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 출력정보등록 팝업 HTML
 * 2016/2/19 출력판 옵션내용 수정 (굿프린팅 김동현)
 */
function outputRegiPopupHtml($param, $val) {

    $tmp = explode("♪", $param);

    $html  = commonRegiPopupHtml("상품출력정보");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n        <div class=\"form-group\">";
    $html .= "\n               <label class=\"control-label fix_width75 tar\">출력명</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_output_name\" value=\"" . $val["output_name"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">출력판</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_output_board_dvs\">";
    $html .= $tmp[2];
    $html .= "\n           </select>";
    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("output", $tmp[0], $tmp[1]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 사이즈정보등록 팝업 HTML
 */
function sizeRegiPopupHtml($param, $val) {

    $tmp = explode("♪", $param);
      
    $html  = commonRegiPopupHtml("상품사이즈품목관리");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n       <div class=\"form-group\">";
    $html .= "\n           <label class=\"control-label fix_width83 tar\">사이즈 대분류</label><label class=\"fix_width10 fs14 tac\">:</label>  ";
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_size_sort\">";
    $html .= $tmp[0];
    $html .= "\n           </select>";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">계열</label><label class=\"fix_width10 fs14 tac\">:</label>  ";
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_size_affil\">";
    $html .= "\n             <option value=\"46\">46</option>";
    $html .= "\n             <option value=\"국\">국</option>";
    $html .= "\n             <option value=\"A3\">A3</option>";
    $html .= "\n             <option value=\"별\">별</option>";
    $html .= "\n             <option value=\"\">없음</option>";
    $html .= "\n           </select>";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width83 tar\">사이즈명</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_size_name\" value=\"" . $val["name"] . "\" maxlength=\"25\">";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">사이즈유형</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_size_typ\" value=\"" . $val["typ"] . "\" maxlength=\"50\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width83 tar\">출력명</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_size_output_name\" onchange=\"changeOutputName(this.value);\">";
    $html .= $tmp[1];
    $html .= "\n           </select>";
    $html .= "\n           <label class=\"control-label fix_width75 tar\">출력판구분</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_size_output_board_dvs\">";
    $html .= $tmp[2];
    $html .= "\n           </select>";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width83 tar\">작업사이즈</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_size_work_wid_size\" value=\"" . $val["work_wid_size"] . "\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <label class=\"fix_width10 fs14 tac\">*</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_size_work_vert_size\" value=\"" . $val["work_vert_size"] . "\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width83 tar\">재단사이즈</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_size_cut_wid_size\" value=\"" . $val["cut_wid_size"] . "\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <label class=\"fix_width10 fs14 tac\">*</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_size_cut_vert_size\" value=\"" . $val["cut_vert_size"] . "\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width83 tar\">디자인사이즈</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_size_design_wid_size\" value=\"" . $val["design_wid_size"] . "\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <label class=\"fix_width10 fs14 tac\">*</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_size_design_vert_size\" value=\"" . $val["design_vert_size"] . "\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width83 tar\">도무송사이즈</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_size_tomson_wid_size\" value=\"" . $val["tomson_wid_size"] . "\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <label class=\"fix_width10 fs14 tac\">*</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" id=\"pop_size_tomson_vert_size\" value=\"" . $val["tomson_vert_size"] . "\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";

    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("size", $tmp[3], $tmp[4]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 상품인쇄정보등록 팝업 HTML
 */
function printRegiPopupHtml($param, $val) {

    $tmp = explode("♪", $param);

    $html  = commonRegiPopupHtml("상품인쇄정보");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n        <div class=\"form-group\">";
    $html .= "\n               <label class=\"control-label fix_width100 tar\">인쇄명</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2\" style=\"width:143px;\" placeholder=\"\" id=\"pop_print_name\" value=\"" . $val["print_name"] . "\" maxlength=\"20\">";
    $html .= "\n            <label class=\"control-label fix_width100 tar\">용도구분</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2\" style=\"width:143px;\" placeholder=\"\" id=\"pop_print_purp_dvs\" value=\"" . $val["purp_dvs"] . "\" maxlength=\"20\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">계열</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width150\" id=\"pop_print_affil\">";
    $html .= "\n               <option value=\"\">없음</option>";
    $html .= "\n               <option value=\"46\">46</option>";
    $html .= "\n               <option value=\"국\">국</option>";
    $html .= "\n             <option value=\"A3\">A3</option>";
    $html .= "\n           </select>";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">기준단위</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select style=\"width:147px;\" id=\"pop_print_crtr_unit\">";
    $html .= "\n             <option value=\"장\">장</option>";
    $html .= "\n             <option value=\"R\">R</option>";
    $html .= "\n           </select>";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">카테고리</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width150\" id=\"pop_print_cate_top\" onchange=\"cateSelect(this.value);\">";
    $html .= $tmp[0];
    $html .= "\n           </select>";
    $html .= "\n           <select class=\"fix_width150\" id=\"pop_print_cate_mid\">";
    $html .= $tmp[1];
    $html .= "\n           </select>";
    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("print", $tmp[2], $tmp[3]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 상품인쇄도수정보등록 팝업 HTML
 */
function tmptRegiPopupHtml($param, $val) {

    $tmp = explode("♪", $param);

    $html  = commonRegiPopupHtml("상품인쇄도수관리");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n       <div class=\"form-group\">";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">인쇄도수대분류</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_tmpt_sort\">";
    $html .= $tmp[0];
    $html .= "\n           </select>";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">인쇄도수명</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" value=\"" . $val["name"] . "\" id=\"pop_tmpt_name\" maxlength=\"20\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">인쇄명</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_tmpt_print_name\" onchange=\"changePrintName(this.value);\">";
    $html .= $tmp[1];
    $html .= "\n           </select>";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">용도구분</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_tmpt_purp_dvs\">";
    $html .= $tmp[2];
    $html .= "\n           </select>";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">면구분</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width147\" id=\"pop_tmpt_side_dvs\">";
    $html .= "\n             <option value=\"단면\">단면</option>";
    $html .= "\n             <option value=\"양면\">양면</option>";
    $html .= "\n             <option value=\"전면\">전면</option>";
    $html .= "\n             <option value=\"후면\">후면</option>";
    $html .= "\n             <option value=\"전면추가\">전면추가</option>";
    $html .= "\n             <option value=\"후면추가\">후면추가</option>";
    $html .= "\n           </select>";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">앞면인쇄</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" value=\"" . $val["beforeside_tmpt"] . "\" id=\"pop_tmpt_beforeside_tmpt\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">뒷면인쇄</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" value=\"" . $val["aftside_tmpt"] . "\" id=\"pop_tmpt_aftside_tmpt\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">추가인쇄</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" value=\"" . $val["add_tmpt"] . "\" id=\"pop_tmpt_add_tmpt\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">총도수</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" value=\"" . $val["tot_tmpt"] . "\" id=\"pop_tmpt_tot_tmpt\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">총판수</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width140\" placeholder=\"\" value=\"" . $val["output_board_amt"] . "\" id=\"pop_tmpt_output_board_amt\" onkeyup=\"onlyNumber2(event);\" maxlength=\"13\">";
    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("tmpt", $tmp[3], $tmp[4]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 후공정관리등록 팝업 HTML
 */
function afterRegiPopupHtml($param, $val) {

    $tmp = explode("♪", $param);

    $html  = commonRegiPopupHtml("상품후공정관리");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n       <div class=\"form-group\">";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">후공정명</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width100\" id=\"pop_after_name\">";
    $html .= $tmp[0];
    $html .= "\n           </select>";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">Depth1</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width94\" placeholder=\"\" id=\"pop_after_depth1\" value=\"" . $val["depth1"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width94 tar\">Depth2</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width94\" placeholder=\"\" id=\"pop_after_depth2\" value=\"" . $val["depth2"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width94 tar\">Depth3</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width100\" placeholder=\"\" id=\"pop_after_depth3\" value=\"" . $val["depth3"] . "\" maxlength=\"20\">";
    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("after", $tmp[1], $tmp[2]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 옵션관리등록 팝업 HTML
 */
function optRegiPopupHtml($param, $val) {

    $tmp = explode("♪", $param);
    
    $html  = commonRegiPopupHtml("상품옵션관리");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n       <div class=\"form-group\">";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">옵션명</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <select class=\"fix_width100\" id=\"pop_opt_name\">";
    $html .= $tmp[0];
    $html .= "\n           </select>";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">Depth1</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width94\" placeholder=\"\" id=\"pop_opt_depth1\" value=\"" . $val["depth1"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width94 tar\">Depth2</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width94\" placeholder=\"\" id=\"pop_opt_depth2\" value=\"" . $val["depth2"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width94 tar\">Depth3</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width100\" placeholder=\"\" id=\"pop_opt_depth3\" value=\"" . $val["depth3"] . "\" maxlength=\"20\">";
    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("opt", $tmp[1], $tmp[2]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 실사 후공정관리등록 팝업 HTML
 */
function aoAfterRegiPopupHtml($param, $val) {

    $tmp = explode("♪", $param);

    $html  = commonRegiPopupHtml("상품실사후공정관리");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n       <div class=\"form-group\">";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">후공정명</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width94\" placeholder=\"\" id=\"pop_ao_after_name\" value=\"" . $val["after_name"] . "\" maxlength=\"20\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">Depth1</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width94\" placeholder=\"\" id=\"pop_ao_after_depth1\" value=\"" . $val["depth1"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width94 tar\">Depth2</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width94\" placeholder=\"\" id=\"pop_ao_after_depth2\" value=\"" . $val["depth2"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width94 tar\">Depth3</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width100\" placeholder=\"\" id=\"pop_ao_after_depth3\" value=\"" . $val["depth3"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width94 tar\">단가</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width100\" placeholder=\"\" id=\"pop_ao_after_unitprice\" value=\"" . $val["unitprice"] . "\" maxlength=\"20\">";
    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("ao_after", $tmp[1], $tmp[2]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}

/**
 * @brief 실사 옵션관리등록 팝업 HTML
 */
function aoOptRegiPopupHtml($param, $val) {

    $tmp = explode("♪", $param);
    
    $html  = commonRegiPopupHtml("상품실사옵션관리");
    $html .= "\n<div class=\"pop-base\">";
    $html .= "\n   <div class=\"pop-content\">";
    $html .= "\n       <div class=\"form-group\">";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">옵션명</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width94\" placeholder=\"\" id=\"pop_ao_opt_name\" value=\"" . $val["opt_name"] . "\" maxlength=\"20\">";
    $html .= "\n           <br />";
    $html .= "\n           <label class=\"control-label fix_width100 tar\">Depth1</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width94\" placeholder=\"\" id=\"pop_ao_opt_depth1\" value=\"" . $val["depth1"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width94 tar\">Depth2</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width94\" placeholder=\"\" id=\"pop_ao_opt_depth2\" value=\"" . $val["depth2"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width94 tar\">Depth3</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width100\" placeholder=\"\" id=\"pop_ao_opt_depth3\" value=\"" . $val["depth3"] . "\" maxlength=\"20\">";
    $html .= "\n           <label class=\"control-label fix_width94 tar\">단가</label><label class=\"fix_width10 fs14 tac\">:</label>";
    $html .= "\n           <input type=\"text\" class=\"input_co2 fix_width100\" placeholder=\"\" id=\"pop_ao_opt_unitprice\" value=\"" . $val["unitprice"] . "\" maxlength=\"20\">";
    $html .= "\n       </div>";
    $html .= commonRegiPopupBtnHtml("ao_opt", $tmp[1], $tmp[2]);
    $html .= "\n   </div>";
    $html .= "\n</div>";

    return $html;
}
?>
