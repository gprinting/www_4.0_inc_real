<?
//상품정보 html
function getPrdtInfoHtml($param) {

    $html = <<<VIEWHTML

    <form name="prdt_form" id="prdt_form" method="post">

        <div id="prdt_info">    
            <table class="table_search_layout">
                <tr>
                    <td colspan="2">
                        <hr class="section_bar">
                    </td>
                </tr>
                <tr>
                    <th class="th_text th_data_prdt_info_01">품절 여부</th>
                    <td>
                        <label class="check_box_wrapper_prdt_info_01"><input type="checkbox" class="checkbox_align" id="gen_y" name="gen_y" value="Y" $param[gen_y]> 일반상품으로 표시</label>
                        <label class="check_box_wrapper_prdt_info_01"><input type="checkbox" class="checkbox_align" id="prjt_y" name="prjt_y" value="Y" $param[prjt_y]> 기획상품으로 표시</label>
                        <label class="check_box_wrapper_prdt_info_01"><input type="checkbox" class="checkbox_align" id="new_y" name="new_y" value="Y" $param[new_y]> 신상품으로 표시</label>
                        <label class="check_box_wrapper_prdt_info_01"><input type="checkbox" class="checkbox_align" id="recomm_y" name="recomm_y" value="Y" $param[recomm_y]> 추천상품으로 표시</label>

                    </td>
                </tr>
                <tr>
                    <th class="th_text th_data_prdt_info_01"></th>
                    <td>
                        <label class="check_box_wrapper_prdt_info_01"><input type="checkbox" class="checkbox_align" id="soldout_y" name="soldout_y" value="Y" $param[soldout_y]> 품절상품으로 표시</label>
                        <label class="check_box_wrapper_prdt_info_01"><input type="checkbox" class="checkbox_align" id="popular_y" name="popular_y" value="Y" $param[popular_y]> 인기상품으로 표시</label>
                        <label class="check_box_wrapper_prdt_info_01"><input type="checkbox" class="checkbox_align" id="event_y" name="event_y" value="Y" $param[event_y]> 이벤트상품으로 표시</label>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <hr class="section_bar">
                    </td>
                </tr>
                <tr>
                    <th class="th_text th_data_prdt_info_01">구분</th>
                    <td>
                        <label class="check_box_wrapper_prdt_info_01"><input type="radio" name="bun_yn" value="Y" class="checkbox_align" $param[bun_y]>묶음가능</label>
                        <label class="check_box_wrapper_prdt_info_01"><input type="radio" name="bun_yn" value="N" class="checkbox_align" $param[bun_n]>묶음불가능</label>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <hr class="section_bar">
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <div class="pop-base fl fix_width1150" style="padding-top:20px;">
                            <div class="pop-content fix_height415">
                                <ul class="form-group">
                                    <li class="fix_width400 fl">
                                        <img id="main_photo" class="process_view_box10" src="$param[photo1]">
                                    </li>
                                    <li class="fix_width120 fl">
                                        <img class="process_view_box11" onclick="changePhoto('$param[photo1]'); return false;" src="$param[photo1]">
                                        <br />
                                        <img class="process_view_box11" onclick="changePhoto('$param[photo2]')" src="$param[photo2]">
                                        <br />
                                        <img class="process_view_box11" onclick="changePhoto('$param[photo3]')" src="$param[photo3]">
                                        <br />
                                        <img class="process_view_box11" onclick="changePhoto('$param[photo4]')" src="$param[photo4]">
                                    </li>
                                    <li class="fl">
                                        <div class="file_div_wrapper">
                                            <label class="th_text th_data_prdt_info_02">이미지</label>
                                            <!-- 파일 업로드 / 스크립이 하단에 있어야 작동함 -->
                                            <input id="upload_file1" class="input_data_prdt_info_01" placeholder="" readonly="readonly" />
                                            <input type="hidden" id="upload_file_check1" name="upload_file_check1">

                                            <label class="fileUpload">
                                                <input id="upload_btn1" name="upload_btn1" type="file" class="upload" />
                                                <span class="btn btn-sm btn-info fa">찾아보기</span>
                                            </label>
                                            <p class="fs13" style="color:red;">* 사이즈 : 1000 x 1000 / 사용가능 확장자 : jpg, gif, png</p>
                                            <script type="text/javascript">
                                                document.getElementById("upload_btn1").onchange = function () {
                                                    document.getElementById("upload_file1").value = this.value;
                                                    document.getElementById("upload_file_check1").value = this.value;
                                                };
                                            </script>
                                            <div id="file_area1">
                                                <label class="fix_width105"> </label>
                                                <label id="file_name1" class="control-label cp blue_text01">$param[file_name1]</label>
                                                <button onclick="delPhotoFile('$param[photo_seqno1]',1)" type="button" id="del_btn1" class="btn btn-sm bred fa" $param[del_btn1]>이미지삭제</button>                        
                                            </div>
                                        </div>
                                        <br />
                                        <div class="file_div_wrapper">
                                            <label class="th_text th_data_prdt_info_02">이미지</label>
                                            <!-- 파일 업로드 / 스크립이 하단에 있어야 작동함 -->
                                            <input id="upload_file2" class="input_data_prdt_info_01" placeholder="" readonly="readonly" />
                                            <input type="hidden" id="upload_file_check2" name="upload_file_check2">

                                            <label class="fileUpload">
                                                <input id="upload_btn2" name="upload_btn2" type="file" class="upload" />
                                                <span class="btn btn-sm btn-info fa">찾아보기</span>
                                            </label>
                                            <p class="fs13" style="color:red;">* 사이즈 : 1000 x 1000 / 사용가능 확장자 : jpg, gif, png</p>
                                            <script type="text/javascript">
                                                document.getElementById("upload_btn2").onchange = function () {
                                                    document.getElementById("upload_file2").value = this.value;
                                                    document.getElementById("upload_file_check2").value = this.value;
                                                };
                                            </script>
                                            <div id="file_area2">
                                                <label class="fix_width105"> </label>
                                                <label id="file_name2" class="control-label cp blue_text01">$param[file_name2]</label>
                                                <button onclick="delPhotoFile('$param[photo_seqno2]',2)" type="button" id="del_btn2" class="btn btn-sm bred fa" $param[del_btn2]>이미지삭제</button>                        
                                            </div>
                                        </div>
                                        <br />
                                        <div class="file_div_wrapper">
                                            <label class="th_text th_data_prdt_info_02">이미지</label>
                                            <!-- 파일 업로드 / 스크립이 하단에 있어야 작동함 -->
                                            <input id="upload_file3" class="input_data_prdt_info_01" placeholder="" readonly="readonly" />
                                            <input type="hidden" id="upload_file_check3" name="upload_file_check3">
                                            <label class="fileUpload">
                                                <input id="upload_btn3" name="upload_btn3" type="file" class="upload" />
                                                <span class="btn btn-sm btn-info fa">찾아보기</span>
                                                
                                            </label>
                                            <p class="fs13" style="color:red;">* 사이즈 : 1000 x 1000 / 사용가능 확장자 : jpg, gif, png</p>
                                            <script type="text/javascript">
                                                document.getElementById("upload_btn3").onchange = function () {
                                                    document.getElementById("upload_file3").value = this.value;
                                                    document.getElementById("upload_file_check3").value = this.value;
                                                };
                                            </script>
                                            <div id="file_area3">
                                                <label class="fix_width105"> </label>
                                                <label class="control-label cp blue_text01" id="file_name3">$param[file_name3]</label>
                                                <button onclick="delPhotoFile('$param[photo_seqno3]',3)" type="button" class="btn btn-sm bred fa" id="del_btn3" $param[del_btn3]>이미지삭제</button>                        
                                            </div>
                                        </div>
                                        <br />
                                        <div class="file_div_wrapper">
                                            <label class="th_text th_data_prdt_info_02">이미지</label>
                                            <!-- 파일 업로드 / 스크립이 하단에 있어야 작동함 -->
                                            <input id="upload_file4" class="input_data_prdt_info_01" placeholder="" readonly="readonly" />
                                            <input type="hidden" id="upload_file_check4" name="upload_file_check4">

                                            <label class="fileUpload">
                                                <input id="upload_btn4" name="upload_btn4" type="file" class="upload" />
                                                <span class="btn btn-sm btn-info fa">찾아보기</span>
                                            </label>
                                            <p class="fs13" style="color:red;">* 사이즈 : 1000 x 1000 / 사용가능 확장자 : jpg, gif, png</p>
                                            <script type="text/javascript">
                                                document.getElementById("upload_btn4").onchange = function () {
                                                    document.getElementById("upload_file4").value = this.value;
                                                    document.getElementById("upload_file_check4").value = this.value;
                                                };
                                            </script>
                                            <div id="file_area4">
                                                <label class="fix_width105"> </label>
                                                <label class="control-label cp blue_text01" id="file_name4">$param[file_name4]</label>
                                                <button onclick="delPhotoFile('$param[photo_seqno4]',4)" type="button" class="btn btn-sm bred fa" id="del_btn4" $param[del_btn4]>이미지삭제</button>                        
                                            </div>
                                        </div>
                                        <br /><br />
                                    </li>
                                </ul>
                            </div>
                        </div>
                    </td>
                </tr>
            </table>

        </div>
        <!-- 탭 박스 3 TAB -->
        <div style="clear:both;">
            <p class="btn-lg red_btn">
                <a onclick="savePrdtInfo()" href="#">선택한 설정을 저장합니다.</a>
            </p>
        </div>
    </form>
VIEWHTML;

    return $html;

}
?>
