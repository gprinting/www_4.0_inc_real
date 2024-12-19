<?
function setContentHtml($param) {

    $html = <<<HTML
            <form name="banner_form$param[count]" id="banner_form$param[count]" method="post">
                <!--  메인배너 컨텐츠 시작 -->
                <div class="pop-base fl fix_width1150" style="padding-top: 20px;">
                    <div class="pop-content" style="height:290px;">
                        <ul class="form-group">
                            <!-- 미리보기 시작-->
                            <li class="fl" style="width:526px;">
                                <div class="process_view_box16" style="width:460px;height:235px;" id="img_div$param[count]">
                                    $param[img_html]
                                </div>
                            </li>
                            <!-- 미리보기 종료-->

                            <!-- 업로드 설정 시작-->
                            <li class="fl">
                                <div class="file_div_wrapper" style="border:none;">
                                    <label class="th_text th_data_mbanner_mng_02">사용유무</label>
                                    <label class="check_box_wrapper_mbanner_mng_01"><input type="radio" name="use_yn$param[count]" value="Y" class="checkbox_align" $param[use_y]>사용</label>
                                    <label class="check_box_wrapper_mbanner_mng_01"><input type="radio" name="use_yn$param[count]" value="N" class="checkbox_align" $param[use_n]>사용안함</label><br />
                                    <label class="th_text th_data_mbanner_mng_02">구분</label>
                                    <label class="check_box_wrapper_mbanner_mng_01"><input type="radio" name="banner_dvs$param[count]" value="공지" class="checkbox_align" $param[dvs1]>공지</label>
                                    <label class="check_box_wrapper_mbanner_mng_02"><input type="radio" name="banner_dvs$param[count]" value="상품소개" class="checkbox_align" $param[dvs2]>상품소개</label>
                                    <label class="check_box_wrapper_mbanner_mng_01"><input type="radio" name="banner_dvs$param[count]" value="이벤트" class="checkbox_align" $param[dvs3]>이벤트</label><br />

                                    <label class="th_text th_data_mbanner_mng_02">이미지</label>
                                    <!-- 파일 업로드 / 스크립이 하단에 있어야 작동함 -->
                                    <input id="upload_file$param[count]" name="upload_file$param[count]" class="select_data_mbanner_mng_01" placeholder="" disabled="disabled" />

                                    <label class="fileUpload">
                                        <input name="upload_btn$param[count]" id="upload_btn$param[count]" type="file" class="upload" />
                                        <span class="btn btn-sm btn-info fa">찾아보기</span>
                                    </label>
                                    <p class="fs13" style="color:red;">* 사이즈 : 1000 x 510 / 사용가능 확장자 : jpg, gif, png</p>
                                    <script type="text/javascript">
                                        document.getElementById("upload_btn$param[count]").onchange = function () {
                                        document.getElementById("upload_file$param[count]").value = this.value;
                                            };
                                    </script>
                                    <div id="file_area$param[count]">
                                        <label class="fix_width105"> </label>
                                        <label class="control-label cp blue_text01" id="file_name$param[count]">
                                            $param[origin_file_name]
                                        </label>
                                    </div>

                                    <label class="th_text th_data_mbanner_mng_02">링크 URL</label>
                                    <input type="text" name="url_addr$param[count]" class="input_co2 fix_width270" value="$param[url_addr]">
                                    <input type="hidden" id="banner_seqno$param[count]" value="$param[seqno]">
                                    <div>
                                        <p class="btn-lg red_btn">
                                            <a onclick="savePopupSet('$param[count]'); return false;" href="#">저장</a>
                                        </p>
                                    </div>
                                </div>
                            </li>
                            <!-- 업로드 설정 종료-->
                        </ul>
                    </div>
                </div>
            </form>
            <hr class="section_bar">
            <!--  메인배너 컨텐츠 종료-->

HTML;

    return $html;
}
?>
