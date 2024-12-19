<?
//웹 기본가입정보
function stanJoinInfo($stan_param) {
    if ($stan_param["join_path"] == "Normal") {
        $stan_html = <<<HTML
            <header class="section info">기본 가입정보</header>
            <article class="tableWrap">
                <table class="sheetForm horizontal">
                    <colgroup>
                        <col class="th">
                        <col class="email">
                        <col class="th">
                        <col>
                        <col class="th">
                        <col>
                    </colgroup>
                    <tbody>
                        <tr>
                            <th>아이디</th>
                            <td><span class="text">$stan_param[member_id]</span></td>
                            <th>회원구분</th>
                            <td><span class="text">$stan_param[member_dvs]</span></td>
                            <th>회원등급</th>
                            <td><span class="text">$stan_param[grade_name]</span></td>
                        </tr>
                        <tr>
                            <th>가상계좌</th>
                            <td colspan="5">
                                <span class="text">$stan_param[bank_name] $stan_param[ba_num]</span>
                                $stan_param[change_account]
                                <button onclick="location.href='/mypage22/benefits_virtual_mng.html'" class="function">가상계좌 변경</button>
                            </td>
                        </tr>
                        <tr>
                            <th>기존 비밀번호</th>
                            <td colspan="5">
                                <input type="password" id="exi_pw" maxlength="20">
                            </td>
                        </tr>
                        <tr>
                            <th>신규 비밀번호</th>
                            <td colspan="5">
                                <input type="password" id="new_pw" maxlength="20">
                                <span class="guide">비밀번호는 8~12자리로 설정 가능합니다.</span>
                            </td>
                        </tr>
                        <tr>
                            <th>신규 비밀번호 확인</th>
                            <td colspan="5">
                                <input type="password" id="new_pw_re" maxlength="20">
                                <button onclick="modiPw();" class="function">비밀번호 변경</button>
                                <span class="guide">신규 비밀번호를 한번 더 입력해주세요.</span>
                            </td>
                        </tr>
                    </tbody>
                </table>
            </article>
            <hr class="end">
HTML;

        // 소셜로그인 일때
    } else {
        $stan_html = <<<HTML
            <header class="section info">기본 가입정보</header>
            <article class="tableWrap">
                <table class="sheetForm horizontal">
                    <colgroup>
                        <col class="th">
                        <col>
                        <col class="th">
                        <col>
                    </colgroup>
                    <tbody>
                        <tr>
                            <th>회원구분</th>
                            <td>$stan_param[member_dvs]</td>
                            <th>회원등급</th>
                            <td>$stan_param[grade_name]</td>
                        </tr>
                        <tr>
                            <th>아이디</th>
                            <td>
                                $stan_param[social_icon]&nbsp;
                                $stan_param[member_id]
                            </td>
                            <th>가상계좌</th>
                            <td>
                                $stan_param[bank_name]&nbsp;
                                $stan_param[ba_num]&nbsp;&nbsp;
                                $stan_param[change_account]
                                <!--p class="note">가상계좌 변경요청은 전화상으로만 가능합니다.</p-->
                            </td>
                        </tr>
                    </tbody>
                </table>
            </article>
            <hr class="end">
HTML;


    }

    return $stan_html;

}

//모바일 기본가입정보
function mobileStanJoinInfo($stan_param) {

    if ($stan_param["join_path"] == "Normal") {

        $stan_html = <<<HTML
            <div class="basic_table_wrapper">
                <table class="basic_table">
                    <colgroup>
                        <col style="width:30%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>회원구분</th>
                        <td>$stan_param[member_dvs]</td>
                    </tr>
                    <tr>
                        <th>회원등급</th>
                        <td>$stan_param[grade_name]</td>
                    </tr>
                    <tr>
                        <th>아이디</th>
                        <td>
                            $stan_param[member_id]
                        </td>
                    </tr>
                    <tr>
                        <th>가상계좌</th>
                        <td>
                            $stan_param[bank_name]
                            $stan_param[ba_num]&nbsp;&nbsp;
                            $stan_param[change_account]
                            <!--
                            <button class="btn_mini" type="button" style="width:25%; height:24px;">계좌변경</button>
                            -->
                            <br><span class="note_color">가상계좌 변경요청은 전화상으로만 가능합니다.</span>
                        </td>
                    </tr>
                    <tr>
                        <th>기존비밀번호</th>
                        <td>
                            <input class="basic_input" type="password" id="exi_pw" maxlength="20">
                        </td>
                    </tr>
                    <tr>
                        <th>신규비밀번호</th>
                        <td>
                            <input class="basic_input" type="password" id="new_pw" maxlength="20">
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td class="note_color" style="padding:0 0 10px 0 !important;">8~12자리로 설정 가능합니다.</td>
                    </tr>
                    <tr>
                        <th>비밀번호확인</th>
                        <td>
                            <input class="basic_input" type="password" id="new_pw_re" maxlength="20">
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td class="note_color" style="padding:0 !important;">신규 비밀번호를 한번 더 입력해주세요.</td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <div class="button_wrapper">
                                <button class="mypage_bottom_color_btn_inner" type="button" onclick="modiPw();">비밀번호 변경</button>
                            </div>
                        </td>
                    </tr>
                </table>
            </div>
HTML;
    } else {
        $stan_html = <<<HTML
            <div class="basic_table_wrapper">
                <table class="basic_table">
                    <colgroup>
                        <col style="width:30%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th>회원구분</th>
                        <td class="bottom_border">$stan_param[member_dvs]</td>
                    </tr>
                    <tr>
                        <th>회원등급</th>
                        <td class="bottom_border">$stan_param[grade_name]</td>
                    </tr>
                    <tr>
                        <th>아이디</th>
                        <td class="bottom_border">
                            $stan_param[social_icon]&nbsp;
                            $stan_param[member_id]
                        </td>
                    </tr>
                    <tr>
                        <th>가상계좌</th>
                        <td class="bottom_border">
                            $stan_param[bank_name]
                            $stan_param[ba_num]&nbsp;&nbsp;
                            $stan_param[change_account]
                            <!--
                            <button class="btn_mini" type="button" style="width:25%; height:24px;">계좌변경</button>
                            -->
                            <br><span class="note_color">가상계좌 변경요청은 전화상으로만 가능합니다.</span>
                        </td>
                    </tr>
                </table>
            </div>
HTML;
    }

    return $stan_html;
}

//웹 개인가입정보
function perJoinInfo($param) {
    $html = <<<HTML
    
    <header class="section person">개인 가입정보</header>
    <article class="tableWrap">
        <table class="sheetForm horizontal">
            <colgroup>
                <col class="th">
                <col>
            </colgroup>
            <tbody>
                <tr>
                    <th>회원명</th>
                    <td><span>$param[member_name]</span></td>
                </tr>
                <tr>
                    <th>이메일</th>
                    <td class="email _replyToEmail">
                        <ol class="email">
                            <li><input type="text" class="_id" id="email_addr" name="email_addr" value="$param[email_addr]"></li>
                            <li>@</li>
                            <li><input type="text" class="_domain" id="email_domain" name="email_domain" value="$param[email_domain]"><input type="hidden" id="pre_domain" value="$param[email_domain]"></li>
                            <li>
                                <select>
                                    <option class="_custom">직접입력</option>
                                    $param[email]
                                </select>
                            </li>
                        </ol>
                    </td>
                </tr>
                <tr>
                    <th>전화번호</th>
                    <td class="telNum">
                        <ol class="tel">
                            <li>
                                <select id="tel_num1" name="tel_num1">
                                    $param[tel]
                                </select>
                            </li>
                            <li><input type="text" id="tel_num2" name="tel_num2" value="$param[tel_num2]" maxlength="4"></li>
                            <li><input type="text" id="tel_num3" name="tel_num3" value="$param[tel_num3]" maxlength="4"></li>
                        </ol>
                    </td>
                </tr>
                <tr>
                    <th>휴대전화</th>
                    <td class="telNum">
                        <ol class="tel">
                            <li>
                                <select id="cel_num1" name="cel_num1">
                                    $param[cel]
                                </select>
                            </li>
                            <li>
                                <input type="text" id="cel_num2" name="cel_num2" value="$param[cel_num2]" maxlength="4">
                            </li>
                            <li>
                                <input type="text" id="cel_num3" name="cel_num3" value="$param[cel_num3]" maxlength="4">
                            </li>
                        </ol>
                    </td>
                </tr>
                <tr>
                    <th>주소</th>
                    <td>
                        <ol class="address">
                            <li class="zipCode">
                                <input type="text" id="zipcode" name="zipcode" value="$param[zipcode]" readonly>
                                <button type="button" class="softC" onclick="getPostcode('');">우편번호 찾기</button>
                            </li>
                            <li>
                                <input type="text" class="address" id="addr" name="addr" value="$param[addr]" readonly>
                            </li>
                            <li>
                                <input type="text" class="address" id="addr_detail" name="addr_detail" value="$param[addr_detail]">
                            </li>
                        </ol>
                    </td>
                </tr>
                <tr>
                    <th>배송지 등록여부</th>
                    <td>
                        <ul class="options inline">
                            <li><label class="radio"><input type="radio" name="basic_addr_yn" value="Y" checked="checked"><i><i></i></i> 예</label></li>
                            <li><label class="radio"><input type="radio" name="basic_addr_yn" value="N"><i><i></i></i> 아니오</label></li>
                            <li><span class="guide">입력하신 주소를 기본배송지로 등록합니다.</span></li>
                        </ul>
                    </td>
                </tr>
                <tr>
                    <th>이메일 수신여부</th>
                    <td>
                        <ul class="options inline">
                            <li><label class="radio"><input type="radio" name="mailing_yn" value="Y"$param[email_ck_y]><i><i></i></i> 예</label></li>
                            <li><label class="radio"><input type="radio" name="mailing_yn" value="N"$param[email_ck_n]><i><i></i></i> 아니오</label></li>
                            <li><span class="guide">뉴스레터, 이벤트 안내 등의 정보를 수신합니다.</span></li>
                        </ul>
                    </td>
                </tr>
                <tr>
                    <th>SMS 수신여부</th>
                    <td>
                        <ul class="options inline">
                            <li><label class="radio"><input type="radio" name="sms_yn" value="Y"$param[sms_ck_y]><i><i></i></i> 예</label></li>
                            <li><label class="radio"><input type="radio" name="sms_yn" value="N"$param[sms_ck_n]><i><i></i></i> 아니오</label></li>
                            <li><span class="guide">뉴스레터, 이벤트 안내 등의 정보를 수신합니다.</span></li>
                        </ul>
                    </td>
                </tr>
            </tbody>
        </table>
    </article>
    <hr class="end">

HTML;

    return $html;
}

//웹 개인가입정보
function companyJoinInfo($param) {
    $html = <<<HTML
    
    <header class="section person">사업자 가입정보</header>
    <article class="tableWrap">
        <table class="sheetForm horizontal">
            <colgroup>
                <col class="th">
                <col>
            </colgroup>
            <tbody>
                <tr>
                    <th>기업명</th>
                    <td colspan="2">
                        <span><input type="text" id="corp_name" name="corp_name" value="$param[corp_name]"></span>
                    </td>
                    <th>대표자명</th>
                    <td colspan="3">
                        <span class="text"><input type="text" id="repre_name" name="repre_name" value="$param[repre_name]" maxlength="4"></span>
                    </td>        
                </tr>
                <tr>
                    <th>사업자번호</th>
                    <td class="telNum" colspan="2">
                        <ul style="display: flex;">
                            <li style=""><input type="text" id="crn1" name="crn1" value="$param[crn1]" maxlength="3" style="width: 60px;"></li>
                            <li><span style="margin: 3px;">-</span><input type="text" id="crn2" name="crn2" value="$param[crn2]" maxlength="2" style="width: 40px;"></li>
                            <li><span style="margin: 3px;">-</span><input type="text" id="crn3" name="crn3" value="$param[crn3]" maxlength="5" style="width: 90px;"></li>
                        </ul>
                    </td>
                    <th>업태</th>
                    <td><input type="text" id="bc" name="bc" value="$param[bc]"></td>
                    <th>종목</th>
                    <td><input type="text" id="tob" name="tob" value="$param[tob]"></td>     
                </tr>
                <tr>
                    <th>계산서 발행 이메일</th>
                    <td class="email _replyToEmail" colspan="6">
                        <ol class="email">
                            <li><input type="text" class="_id" id="email_addr2" name="email_addr2" value="$param[email_addr]"></li>
                            <li>@</li>
                            <li><input type="text" class="_domain" id="email_domain2" name="email_domain2" value="$param[email_domain]"><input type="hidden" id="pre_domain" value="$param[email_domain]"></li>
                            <li>
                                <select>
                                    <option class="_custom">직접입력</option>
                                    $param[email]
                                </select>
                            </li>
                        </ol>
                    </td>
                </tr>
                <tr>
                    <th>주소</th>
                    <td colspan="6">
                        <ol class="address">
                            <li class="zipCode">
                                <input type="text" id="zipcode2" name="zipcode2" value="$param[zipcode]" readonly>
                                <button type="button" class="softC" onclick="getPostcode2('');">우편번호 찾기</button>
                            </li>
                            <li>
                                <input type="text" class="address" id="addr2" name="addr2" value="$param[addr]" readonly>
                            </li>
                            <li>
                                <input type="text" class="address" id="addr_detail2" name="addr_detail2" value="$param[addr_detail]">
                            </li>
                        </ol>
                    </td>
                </tr>
            </tbody>
        </table>
    </article>
    <hr class="end">

HTML;

    return $html;
}


// 모바일 개인가입정보
function mobilePerJoinInfo($param) {

    $html = <<<HTML
        
        <div class="sub_title_wrapper">
            <div class="sub_title">
                개인 가입정보
            </div>
            </div>
            <div class="basic_table_wrapper">
                <table class="basic_table">
                    <colgroup>
                        <col style="width:30%;">
                        <col style="">
                    </colgroup>
                    <tr>
                        <th class="">회원명</th>
                        <td class="">
                            <span>$param[member_name]</span>
                        </td>
                    </tr>
                    <tr>
                        <th>이메일</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:45%;">
                                    <col style="width:10%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody class="email _replyToEmail">
                                    <tr>
                                        <td>
                                            <input class="basic_input Column_03" type="text" id="email_addr" name="email_addr" value="$param[email_addr]">
                                        </td>
                                        <td style="text-align:center;">@</td>
                                        <td>
                                            <input type="text" class="basic_input Column_03" id="email_domain" name="email_domain" value="">
                                        </td>
                                    </tr>
                                    <tr>
                                        <input type="hidden" id="pre_domain" value="$param[email_domain]">
                                        <td colspan="3" style="padding:4% 0 0 0 !important;">
                                            <select class="basic_input">
                                                <option class="_custom">직접입력</option>
                                                $param[email]
                                            </select>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th>전화번호</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:30%;">
                                    <col style="width:40%;">
                                    <col style="width:30%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <select id="tel_num1" name="tel_num1" class="basic_input Column_03">
                                                $param[tel]
                                            </select>
                                        </td>
                                        <td style="padding:0% 5% !important;">
                                            <input class="basic_input Column_03" type="text" id="tel_num2" name="tel_num2" value="$param[tel_num2]" maxlength="4">
                                        </td>
                                        <td>
                                            <input class="basic_input Column_03" type="text" id="tel_num3" name="tel_num3" value="$param[tel_num3]" maxlength="4">
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th>휴대전화</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:30%;">
                                    <col style="width:40%;">
                                    <col style="width:30%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <select id="cel_num1" name="cel_num1" class="basic_input Column_03">
                                                $param[cel]
                                            </select>
                                        </td>
                                        <td style="padding:0% 5% !important;">
                                            <input class="basic_input Column_03" type="text" id="cel_num2" name="cel_num2" value="$param[cel_num2]" maxlength="4">
                                        </td>
                                        <td>
                                            <input class="basic_input Column_03" type="text" id="cel_num3" name="cel_num3" value="$param[cel_num3]" maxlength="4">
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>

                    <!-- 주소 -->
                    <tr>
                        <th>주소</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:100%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input class="basic_input Column_02" type="text" id="zipcode" name="zipcode" value="$param[zipcode]" readonly="readonly" style="width:40%; margin-right:5%;">
                                            <button class="btn_mini" type="button" style="width:40%; font-size:1em !important;" onclick="getPostcode('');">우편번호 찾기</button>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <input class="basic_input" type="text" id="addr" name="addr" value="$param[addr]" readonly="readonly">
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <input class="basic_input" type="text" id="addr_detail" name="addr_detail" value="$param[addr_detail]">
                        </td>
                    </tr>
                    <!-- 주소 end -->

                    <!-- 알림 수신 -->
                    <tr>
                        <th>알림 수신</th>
                        <td colspan="2">
                            <p style="color:#535353;">뉴스, 이벤트 등 정보를 수신합니다.</p>
                        </td>
                    </tr>

                    <!-- 메일 수신 -->
                    <tr>
                        <th>이메일 수신</thtd>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="radio" id="mail_yes" name="mailing_yn" value="Y"$param[email_ck_y]><label for="mail_yes" class="Column_02">예</label>
                                        </td>
                                        <td>
                                            <input type="radio" id="mail_no" name="mailing_yn" value="N"$param[email_ck_n]><label for="mail_no" class="Column_02">아니오</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>

                    <!-- SMS 수신 -->
                    <tr>
                        <th>SMS 수신</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="radio" id="sms_yes" name="sms_yn" value="Y"$param[sms_ck_y]><label for="sms_yes" class="Column_02">예</label>
                                        </td>
                                        <td>
                                            <input type="radio" id="sms_no" name="sms_yn" value="N"$param[sms_ck_n]><label for="sms_no" class="Column_02">아니오</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                </table>
            </div>

HTML;

    return $html;

}

//사업자 등록정보 $param[description]</h4>
function comJoinInfo($param) {

    $html = <<<HTML
    
        <header class="section company">사업자 등록번호</header>
        <article class="tableWrap">
            <table class="sheetForm horizontal">
                <colgroup>
                    <col class="th">
                    <col>
                    <col class="th">
                    <col>
                </colgroup>
                <tbody>
                    <tr>
                        <th>기업명</th>
                        <td><input type="text" id="corp_name" value="$param[corp_name]" $param[readonly]></td>
                        <th>사업자등록번호</th>
                        <td><input type="text" id="crn" value="$param[crn]" $param[readonly]></td>
                    </tr>
                    <tr>
                        <th>대표자</th>
                        <td colspan="3"><input type="text" id="repre_name" value="$param[repre_name]"></td>
                    </tr>
                    <tr>
                        <th>업태</th>
                        <td><input type="text" id="bc" value="$param[bc]"></td>
                        <th>종목</th>
                        <td><input type="text" id="tob" value="$param[tob]"></td>
                    </tr>
                    <tr>
                        <th>주소</th>
                        <td colspan="3">
                            <div class="rowWrap postNum">
                                <input type="text" id="co_zipcode" name="co_zipcode" value="$param[co_zipcode]" readonly>
                                <button type="button" onclick="getPostcode('co_');">우편번호 찾기</button>
                            </div>
                            <div class="rowWrap address">
                                <input type="text" class="address" id="co_addr" name="co_addr" value="$param[co_addr]" readonly>
                                <input type="text" class="address" id="co_addr_detail" name="co_addr_detail" value="$param[co_addr_detail]">
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
        </article>
        <hr class="end">
HTML;

    return $html;
}

//모바일 사업자등록정보
function mobileComJoinInfo($param) {
    $html = <<<HTML
        <div class="sub_title_wrapper">
            <div class="sub_title">
                사업자 등록정보
            </div>
        </div>
        <div class="basic_table_wrapper">
            <table class="basic_table">
                <colgroup>
                    <col style="width:30%;">
                    <col style="">
                </colgroup>
                <tr>
                    <th>기업명</th>
                    <td>
                        <input class="basic_input" type="password">
                    </td>
                </tr>
                <tr>
                    <th>사업자 번호</th>
                    <td>
                        <input class="basic_input" type="password">
                    </td>
                </tr>
                <tr>
                    <th>대표자</th>
                    <td>
                        <input class="basic_input" type="password">
                    </td>
                </tr>
                <tr>
                    <th>업태</th>
                    <td>
                        <input class="basic_input" type="password">
                    </td>
                </tr>
                <tr>
                    <th>종목</th>
                    <td>
                        <input class="basic_input" type="password">
                    </td>
                </tr>

                <!-- 주소 -->
                <tr>
                    <th>주소</th>
                    <td>
                        <table class="basic_detail_table">
                            <colgroup>
                                <col style="width:100%;">
                            </colgroup>
                            <tbody>
                                <tr>
                                    <td>
                                        <input class="basic_input Column_02" type="text" readonly="readonly" style="width:40%; margin-right:5%;">
                                        <button class="btn_mini" type="button" style="width:40%; font-size:1em !important;">우편번호 찾기</button>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </td>
                </tr>
                <tr>
                    <th></th>
                    <td>
                        <input class="basic_input" type="text" value="서울 강북구 방학로 384 (우이동)" readonly="readonly">
                    </td>
                </tr>
                <tr>
                    <th></th>
                    <td>
                        <input class="basic_input" type="text">
                    </td>
                </tr>
                <!-- 주소 end -->

            </table>
        </div>
HTML;

    return $html;

}

//주문담당자
function orderMng($param) {

    $html = <<<HTML
    <header class="section">주문담당자</header>
    <article class="tableWrap">
        <table class="list info">
            <colgroup>
                <col class="no">
                <col class="name">
                <col class="id">
                <col class="tel">
                <col class="tel">
                <col>
                <col class="btn">
            </colgroup>
            <tbody>
                <tr>
                    <th class="center">번호</th>
                    <th>성명</th>
                    <th>아이디</th>
                    <th>전화번호</th>
                    <th>휴대전화</th>
                    <th>이메일</th>
                    <th class="center">관리</th>
                </tr>
            </tbody>
            <tbody id="order_mng_list">
                $param[order_mng_html]
            </tbody>
        </table>
    </article>
    <footer class="action">
        <button onclick="layerPopup('l_officer', 'popup/l_orderofficer_new.html');">+ 주문담당자 추가</buttozn>
    </footer>
    <hr class="end">
HTML;

    return $html;
}

//회계담당자
function acctingMng($param) {

    $html = <<<HTML
    <header class="section">회계담당자</header>
    <article class="tableWrap">
        <table class="list info">
            <colgroup>
                <col class="no">
                <col class="name">
                <col class="tel">
                <col class="tel">
                <col>
                <col class="btn">
            </colgroup>
            <tbody>
                <tr>
                    <th class="center">번호</th>
                    <th>성명</th>
                    <th>전화번호</th>
                    <th>휴대전화</th>
                    <th>이메일</th>
                    <th class="center">관리</th>
                </tr>
            </tbody>
            <tbody id="accting_mng_list">
                $param[accting_mng_html]
            </tbody>
        </table>
    </article>
    <footer class="action">
        <button onclick="layerPopup('l_officer', 'popup/l_accountofficer_new.html');">+ 회계담당자 추가</buttozn>
    </footer>
    <hr class="end">
HTML;

    return $html;
}

//성향정보
function memberDetail($param) {

    $html = <<<HTML
    <header class="section character">성향정보</header>
    <article class="tableWrap">
        <table class="sheetForm horizontal">
            <colgroup>
                <col class="th">
                <col>
            </colgroup>
            <tbody>
                <tr id="occu_html">
                    <th>회원직업</th>
                    <td>
                        <ol class="depths col6">
                            <li>
                                <select id="occu1" name="occu1" onchange="setOccu2()">
                                    <option>IT 전산개발</option>
                                </select>
                                <input type="hidden" id="pre_occu1" value="$param[occu1]">
                            </li>
                            <li>
                                <select id="occu2" name="occu2">
                                    <option>영업</option>
                                </select>
                                <input type="hidden" id="pre_occu2" value="$param[occu2]">
                            </li>
                            <li class="rest">
                                <input type="text" class="detail" id="occu_detail" name="occu_detail" value="$param[occu_detail]" placeholder="세부적으로 적어주세요.">
                            </li>
                        </ol>
                    </td>
                </tr>
                <tr>
                    <th>관심분야</th>
                    <td>
                        <ol class="depths col6">
                            <li>
                                <select id="interest_field1" name="interest_field1">
                                    <option>요식업종</option>
                                </select>
                                <input type="hidden" id="pre_interest_field1" value="$param[interest_field1]">
                            </li>
                            <li>
                                <select id="interest_field2" name="interest_field2">
                                    <option>한식</option>
                                </select>
                                <input type="hidden" id="pre_interest_field2" value="$param[interest_field2]">
                            </li>
                            <li class="rest">
                                <input type="text" class="detail" id="interest_field_detail" name="interest_field_detail" value="$param[interest_field_detail]" placeholder="세부적으로 적어주세요.">
                            </li>
                        </ol>
                    </td>
                </tr>
                <tr>
                    <th>관심상품</th>
                    <td>
                        <ul class="options inline col6">
                            <li><label class="check"><input type="checkbox" id="inter_prdt1" name="inter_prdt1"$param[inter_prdt1]><i><i></i></i> 명함</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt2" name="inter_prdt2"$param[inter_prdt2]><i><i></i></i> 스티커</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt3" name="inter_prdt3"$param[inter_prdt3]><i><i></i></i> 전단</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt4" name="inter_prdt4"$param[inter_prdt4]><i><i></i></i> 광고홍보물</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt5" name="inter_prdt5"$param[inter_prdt5]><i><i></i></i> 봉투</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt6" name="inter_prdt6"$param[inter_prdt6]><i><i></i></i> 마스터(경인쇄)</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt7" name="inter_prdt7"$param[inter_prdt7]><i><i></i></i> 초소량인쇄</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt8" name="inter_prdt8"$param[inter_prdt8]><i><i></i></i> 디지털인쇄</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt9" name="inter_prdt9"$param[inter_prdt9]><i><i></i></i> 청첩장/초대장</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt10" name="inter_prdt10"$param[inter_prdt10]><i><i></i></i> 패키지</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt11" name="inter_prdt11"$param[inter_prdt11]><i><i></i></i> 실사출력</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_prdt12" name="inter_prd12"$param[inter_prdt12]><i><i></i></i> 판촉물</label></li>
                        </ul>
                    </td>
                </tr>
                <tr>
                    <th>관심디자인</th>
                    <td>
                        <ul class="options inline col6">
                            <li><label class="check"><input type="checkbox" id="inter_design1" name="inter_design1"$param[inter_design1]><i><i></i></i> 항목1</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_design2" name="inter_design2"$param[inter_design2]><i><i></i></i> 항목2</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_design3" name="inter_design3"$param[inter_design3]><i><i></i></i> 항목3</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_design4" name="inter_design4"$param[inter_design4]><i><i></i></i> 항목4</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_design5" name="inter_design5"$param[inter_design5]><i><i></i></i> 항목5</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_design6" name="inter_design6"$param[inter_design6]><i><i></i></i> 항목6</label></li>
                        </ul>
                    </td>
                </tr>
                <tr>
                    <th>관심우선순위</th>
                    <td>
                        <ul class="options inline col6">
                            <li><label class="radio"><input type="radio" name="interest_prior" value="1"$param[inter_prior1]><i><i></i></i> 가격</label></li>
                            <li><label class="radio"><input type="radio" name="interest_prior" value="2"$param[inter_prior2]><i><i></i></i> 품질</label></li>
                            <li><label class="radio"><input type="radio" name="interest_prior" value="3"$param[inter_prior3]><i><i></i></i> 납기</label></li>
                            <li><label class="radio"><input type="radio" name="interest_prior" value="4"$param[inter_prior4]><i><i></i></i> 서비스</label></li>
                        </ul>
                    </td>
                </tr>
                <!--
                <tr>
                    <th>관심이벤트</th>
                    <td>
                        <ul class="options inline col6">
                            <li><label class="check"><input type="checkbox" id="inter_event1" name="inter_event1"$param[inter_event1]><i><i></i></i> 오특이</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_event2" name="inter_event2"$param[inter_event2]><i><i></i></i> 골라담자</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_event3" name="inter_event3"$param[inter_event3]><i><i></i></i> 요즘바빠요</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_event4" name="inter_event4"$param[inter_event4]><i><i></i></i> 포인트</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_event5" name="inter_event5"$param[inter_event5]><i><i></i></i> 쿠폰</label></li>
                        </ul>
                    </td>
                </tr>
                -->
                <tr>
                    <th>관심요구사항</th>
                    <td>
                        <ul class="options inline col6">
                            <li><label class="check"><input type="checkbox" id="inter_needs1" name="inter_needs1"$param[inter_needs1]><i><i></i></i> 방문요청</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_needs2" name="inter_needs2"$param[inter_needs2]><i><i></i></i> 담당자 지정</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_needs3" name="inter_needs3"$param[inter_needs3]><i><i></i></i> 다양한 제품 출시</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_needs4" name="inter_needs4"$param[inter_needs4]><i><i></i></i> 지속적인 이벤트</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_needs5" name="inter_needs5"$param[inter_needs5]><i><i></i></i> 원활한 A/S</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_needs6" name="inter_needs6"$param[inter_needs6]><i><i></i></i> 사이트 제작 조언</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_needs7" name="inter_needs7"$param[inter_needs7]><i><i></i></i> 프로그램 공동구매</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_needs8" name="inter_needs8"$param[inter_needs8]><i><i></i></i> 커뮤니티 강화</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_needs9" name="inter_needs9"$param[inter_needs9]><i><i></i></i> 정기모임</label></li>
                            <li><label class="check"><input type="checkbox" id="inter_needs10" name="inter_needs10"$param[inter_needs10]><i><i></i></i> 유/무상 교육</label></li>
                        </ul>
                    </td>
                </tr>
                <tr>
                    <th>추가 관심사항</th>
                    <td>
                        <input type="text" id="add_interest_items" name="add_interest_items" value="$param[add_interest_items]" class="full" placeholder="세부적으로 적어주세요.">
                    </td>
                </tr>
                <tr>
                    <th>디자인</th>
                    <td>
                        <ul class="options inline col3">
                            <li><label class="radio"><input type="radio" name="design_outsource_yn" value="Y" $param[design_outsource_y]><i><i></i></i> 디자인 관련 기획 전체를 외주합니다.</label>
                            <li><label class="radio"><input type="radio" name="design_outsource_yn" value="N" $param[design_outsource_n]><i><i></i></i> 디자인은 자체적으로 처리합니다.</label>
                        </ul>
                    </td>
                </tr>
                <tr>
                    <th>생산</th>
                    <td>
                        <ul class="options inline col3">
                            <li><label class="radio"><input type="radio" name="produce_outsource_yn" value="Y" $param[produce_outsource_y]><i><i></i></i> 인쇄 외 모든 품목을 외주합니다.</label>
                            <li><label class="radio"><input type="radio" name="produce_outsource_yn" value="N" $param[produce_outsource_n]><i><i></i></i> 자체적으로 인쇄를 생산합니다.</label>
                        </ul>
                    </td>
                </tr>
                <tr>
                    <th>복수거래유무</th>
                    <td class="_printBusiness">
                        <ul class="options inline col6">
                            <li><label class="radio"><input type="radio" name="plural_deal_yn" value="Y"$param[plural_deal_y]><i><i></i></i> 복수거래 중</label>
                            <li><label class="radio"><input type="radio" name="plural_deal_yn" value="N"$param[plural_deal_n]><i><i></i></i> 단수거래 중</label>
                        </ul>
                    </td>
                </tr>
                <tr>
                    <th>복수거래업체</th>
                    <td class="_printBusinessDetail">
                        <ol class="depths rowWrap col6">
                            <li>
                                <select id="plural_deal_site_name1" name="plural_deal_site_name1"$param[plural_disabled]>
                                    <option value="">- 선택 -</option>
                                    <option value="애드코아">애드코아</option>
                                    <option value="디티피아">디티피아</option>
                                </select>
                                <input type="hidden" id="pre_plural_deal_site_name1" value="$param[plural_deal_site_name1]">
                            </li>
                            <li class="rest">
                                <input type="text" id="plural_deal_site_detail1" name="plural_deal_site_detail1" value="$param[plural_deal_site_detail1]" placeholder="세부적으로 적어주세요."$param[plural_disabled]>
                            </li>
                        </ol>
                        <ol class="depths rowWrap col6">
                            <li>
                                <select id="plural_deal_
                                    <select id="plural_deal_site_name2" name="plural_deal_site_name2"$param[plural_disabled]>
                                        <option value="">- 선택 -</option>
                                        <option value="애드코아">애드코아</option>
                                        <option value="디티피아">디티피아</option>
                                    </select>
                                    <input type="hidden" id="pre_plural_deal_site_name2" value="$param[plural_deal_site_name2]">
                            </li>
                            <li class="rest">
                                <input type="text" id="plural_deal_site_detail2" name="plural_deal_site_detail2" value="$param[plural_deal_site_detail2]" placeholder="세부적으로 적어주세요."$param[plural_disabled]>
                            </li>
                        </ol>
                    </td>
                </tr>
                <tr>
                    <th>메모</th>
                    <td><textarea class="full" rows="5" id="memo" name="memo">$param[memo]</textarea></td>
                </tr>
            </tbody>
        </table>
    </article>
    <hr class="end">
HTML;

    return $html;
}

function memberLink($param) {

    $html = <<<HTML

    <!-- 소셜계정 연동 start -->
    <header class="section link">SNS계정 연동 정보</header>
    <article class="tableWrap">
        <table class="sheetForm horizontal">
            <colgroup>
                <col class="th">
                <col>
            </colgroup>
            <tbody>
                <tr class="naver">
                    <th><img src="/design_template/pub22/images/popup/sns-naver.png" alt="네이버">네이버</th>
                    <td>
                        <div>
                            <span>$param[mail_acc_naver]</span>
                            $param[btn_naver]
                        </div>
                    </td>
                </tr>
                <tr class="google">
                    <th><img src="/design_template/pub22/images/popup/sns-google.png" alt="구글">구글</th>
                    <td>
                        <div>
                            <span>$param[mail_acc_google]</span>
                            $param[btn_google]
                        </div>
                    </td>
                </tr>
                <tr class="kakao" id="link_kakao">
                    <th><img src="/design_template/pub22/images/popup/sns-kakao.png" alt="카카오">카카오</th>
                    <td>
                        <div>
                            <span>$param[mail_acc_kakao]</span>
                            $param[btn_kakao]
                        </div>
                    </td>
                </tr>
                <tr class="facebook" id="link_fb">
                    <th><img src="/design_template/pub22/images/popup/sns-facebook.png" alt="페이스북">페이스북</th>
                    <td>
                        <div>
                            <span>$param[mail_acc_fb]</span>
                            $param[btn_fb]
                        </div>
                    </td>
                </tr>
            </tbody>
        </table>
        <p class="boxM" style="display:none;">IE 8.0 이하에서는 카카오와 페이스북 로그인이 동작하지 않으므로 연동할 수 없습니다.</div>
    </article>
    <hr class="end">
    <!-- 소셜계정 연동 end -->

HTML;

    return $html;
}

function mobileMemberDetail($param) {

    $html = <<<HTML

            <!-- 성향정보 start -->
            <div class="sub_title_wrapper">
                <div class="sub_title option_title" onclick="showMemberDetail();">
                    성향정보<img class="btn_view_option" src="/design_template/m/images/common/btn_view_option_on.png" alt="더보기">
                </div>
                <div class="sub_title option_title_on" style="display:none;" onclick="showMemberDetail();">
                    성향정보<img class="btn_view_option" src="/design_template/m/images/common/btn_view_option_off.png" alt="더보기">
                </div>
                <!-- 성향정보 팁 start -->
                <p class="note_color" style="font-size:0.875em;">※성향 정보를 작성해주시는 회원님께는 5,000 포인트를 적립해 드리며, 작성해주신 정보는 홍보수단으로 활용될 수 있습니다.</p>
                <!-- 성향정보 팁 end -->
            </div>

            <div id="member_det" class="basic_table_wrapper option_table" style="display:none;">
                <table class="basic_table">
                    <colgroup>
                        <col style="width:30%;">
                        <col style="">
                    </colgroup>

                    <!-- 회원직업 -->
                    <tr>
                        <th>회원직업</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="occu1" name="occu1" onchange="setOccu2()">
                                                <option value="경영,사무">경영,사무</option>
                                                <option value="마케팅,무역,유통">마케팅,무역,유통</option>
                                                <option value="영업,고객상담">영업,고객상담</option>
                                                <option value="IT,인터넷">IT,인터넷</option>
                                                <option value="연구개발,설계">연구개발,설계</option>
                                                <option value="생산,제조">생산,제조</option>
                                                <option value="전문,특수직">전문,특수직</option>
                                                <option value="디자인">디자인</option>
                                                <option value="미디어">미디어</option>
                                            </select>
                                        </td>
                                        <input type="hidden" id="pre_occu1" value="$param[occu1]">
                                        <td>
                                            <select class="basic_input Column_02" id="occu2" name="occu2">
                                                <option value="자동차,조선,기계">자동차,조선,기계</option>
                                                <option value="반도체,디스플레이">반도체,디스플레이</option>
                                                <option value="화학,에너지,환경,식품">화학,에너지,환경,식품</option>
                                                <option value="전기,전자,제어">전기,전자,제어</option>
                                                <option value="기계설계,CAD,CAM">기계설계,CAD,CAM</option>
                                                <option value="통신기술,네트워크구축">통신기술,네트워크구축</option>
                                                <option value="건설,설계,인테리어">건설,설계,인테리어</option>
                                            </select>
                                        </td>
                                        <input type="hidden" id="pre_occu2" value="$param[occu2]">
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <input class="basic_input" type="text" id="occu_detail" name="occu_detail" value="$param[occu_detail]" placeholder="세부적으로 적어주세요.">
                        </td>
                    </tr>
                    <!-- 회원직업 end -->

                    <!-- 관심분야 -->
                    <tr>
                        <th>관심분야</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:45%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td style="padding-right:5% !important;">
                                            <select class="basic_input Column_02" id="interest_field1" name="interest_field1">
                                                <option value="">요식업종</option>
                                            </select>
                                        </td>
                                        <input type="hidden" id="pre_interest_field1" value="$param[interest_field1]">
                                        <td>
                                            <select class="basic_input Column_02" id="interest_field2" name="interest_field2">
                                                <option value="">한식</option>
                                            </select>
                                        </td>
                                        <input type="hidden" id="pre_interest_field2" value="$param[interest_field2]">
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <input class="basic_input" type="text" id="interest_field_detail" name="interest_field_detail" value="$param[interest_field_detail]" placeholder="세부적으로 적어주세요.">
                        </td>
                    </tr>
                    <!-- 관심분야 end -->

                    <!-- 관심상품 -->
                    <tr>
                        <th>관심상품</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_prdt1" name="inter_prdt1"$param[inter_prdt1]><label for="inter_prdt1" class="Column_02">명함</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_prdt2" name="inter_prdt2"$param[inter_prdt2]><label for="inter_prdt2" class="Column_02">스티커</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_prdt3" name="inter_prdt3"$param[inter_prdt3]><label for="inter_prdt3" class="Column_02">전단</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_prdt4" name="inter_prdt4"$param[inter_prdt4]><label for="inter_prdt4" class="Column_02">광고홍보물</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_prdt5" name="inter_prdt5"$param[inter_prdt5]><label for="inter_prdt5" class="Column_02">봉투</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_prdt6" name="inter_prdt6"$param[inter_prdt6]><label for="inter_prdt6" class="Column_02">마스터(경인쇄)</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_prdt7" name="inter_prdt7"$param[inter_prdt7]><label for="inter_prdt7" class="Column_02">초소량인쇄</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_prdt8" name="inter_prdt8"$param[inter_prdt8]><label for="inter_prdt8" class="Column_02">디지털인쇄</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_prdt9" name="inter_prdt9"$param[inter_prdt9]><label for="inter_prdt9" class="Column_02">청첩장/초대장</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_prdt10" name="inter_prdt10"$param[inter_prdt10]><label for="inter_prdt10" class="Column_02">패키지</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_prdt11" name="inter_prdt11"$param[inter_prdt11]><label for="inter_prdt11" class="Column_02">실사출력</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_prdt12" name="inter_prdt12"$param[inter_prdt12]><label for="inter_prdt12" class="Column_02">판촉물</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <!-- 관심상품 end -->

                    <!-- 관심디자인 -->
                    <tr>
                        <th>관심디자인</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_design1" name="inter_design1"$param[inter_design1]><label for="inter_design1" class="Column_02">항목1</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_design2" name="inter_design2"$param[inter_design2]><label for="inter_design2" class="Column_02">항목2</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_design3" name="inter_design3"$param[inter_design3]><label for="inter_design3" class="Column_02">항목3</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_design4" name="inter_design4"$param[inter_design4]><label for="inter_design4" class="Column_02">항목4</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_design5" name="inter_design5"$param[inter_design5]><label for="inter_design5" class="Column_02">항목5</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_design6" name="inter_design6"$param[inter_design6]><label for="inter_design6" class="Column_02">항목6</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <!-- 관심디자인 end -->

                    <!-- 관심우선순위 -->
                    <tr>
                        <th>관심우선순위</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="radio" id="interest_prior_01" name="interest_prior" value="1"$param[inter_prior1]><label for="interest_prior_01" class="Column_02">가격</label>
                                        </td>
                                        <td>
                                            <input type="radio" id="interest_prior_02" name="interest_prior" value="2"$param[inter_prior2]><label for="interest_prior_02" class="Column_02">품질</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="radio" id="interest_prior_03" name="interest_prior" value="3"$param[inter_prior3]><label for="interest_prior_03" class="Column_02">납기</label>
                                        </td>
                                        <td>
                                            <input type="radio" id="interest_prior_04" name="interest_prior" value="4"$param[inter_prior4]><label for="interest_prior_04" class="Column_02">서비스</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <!-- 관심우선순위 end -->

                    <!-- 관심이벤트 -->
                    <tr>
                        <th>관심이벤트</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_event_01" name="inter_event1"$param[inter_event1]><label for="inter_event_01" class="Column_02">오특이</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_event_02" name="inter_event2"$param[inter_event2]><label for="inter_event_02" class="Column_02">골라담자</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_event_03" name="inter_event3"$param[inter_event3]><label for="inter_event_03" class="Column_02">요즘바빠요</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_event_04" name="inter_event4"$param[inter_event4]><label for="inter_event_04" class="Column_02">포인트</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_event_05" name="inter_event5"$param[inter_event5]><label for="inter_event_05" class="Column_02">쿠폰</label>
                                        </td>
                                        <td></td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <!-- 관심이벤트 end -->

                    <!-- 관심요구사항 -->
                    <tr>
                        <th>관심요구사항</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_needs1" name="inter_needs1"$param[inter_needs1]><label for="inter_needs1" class="Column_02">방문요청</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_needs2" name="inter_needs2"$param[inter_needs2]><label for="inter_needs2" class="Column_02">담당자 지정</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_needs3" name="inter_needs3"$param[inter_needs3]><label for="inter_needs3" class="Column_02">제품 출시</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_needs4" name="inter_needs4"$param[inter_needs4]><label for="inter_needs4" class="Column_02">이벤트</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_needs5" name="inter_needs5"$param[inter_needs5]><label for="inter_needs5" class="Column_02">원활한 A/S</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_needs6" name="inter_needs6"$param[inter_needs6]><label for="inter_needs6" class="Column_02">사이트 제작</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_needs7" name="inter_needs7"$param[inter_needs7]><label for="inter_needs7" class="Column_02">프로그램구매</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_needs8" name="inter_needs8"$param[inter_needs8]><label for="inter_needs8" class="Column_02">커뮤니티강화</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="checkbox" id="inter_needs9" name="inter_needs9"$param[inter_needs9]><label for="inter_needs9" class="Column_02">정기모임</label>
                                        </td>
                                        <td>
                                            <input type="checkbox" id="inter_needs10" name="inter_needs10"$param[inter_needs10]><label for="inter_needs10" class="Column_02">유/무상 교육</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <!-- 관심요구사항 end -->

                    <!-- 추가 관심사항 -->
                    <tr>
                        <th>추가 관심사항</th>
                        <td>
                            <input class="basic_input" type="text" id="add_interest_items" name="add_interest_items" value="$param[add_interest_items]" placeholder="세부적으로 적어주세요.">
                        </td>
                    </tr>
                    <!-- 추가 관심사항 end -->

                    <!-- 디자인 -->
                    <tr>
                        <th>디자인</th>
                        <td>
                            <table class="basic_detail_table">
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="radio" id="design_outsource_yn1" name="design_outsource_yn" value="Y" $param[design_outsource_y]><label for="design_outsource_yn1">디자인 관련 기획 전체를 외주합니다.</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="radio" id="design_outsource_yn2" name="design_outsource_yn" value="N" $param[design_outsource_n]><label for="design_outsource_yn2">디자인은 자체적으로 처리합니다.</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <!-- 디자인 end -->

                    <!-- 생산 -->
                    <tr>
                        <th>생산</th>
                        <td>
                            <table class="basic_detail_table">
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="radio" id="produce_outsource_yn1" name="produce_outsource_yn" value="Y" $param[produce_outsource_y]><label for="produce_outsource_yn1">인쇄 외 모든 품목을 외주합니다.</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="radio" id="produce_outsource_yn2" name="produce_outsource_yn" value="N" $param[produce_outsource_n]><label for="produce_outsource_yn2">자체적으로 인쇄를 생산합니다.</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <!-- 생산 end -->
                    
                    <!-- 복수거래유무 -->
                    <tr>
                        <th>복수거래유무</th>
                        <td>
                            <table class="basic_detail_table">
                                <colgroup>
                                    <col style="width:50%;">
                                    <col style="width:50%;">
                                </colgroup>
                                <tbody>
                                    <tr>
                                        <td>
                                            <input type="radio" id="plural_deal_yn1" name="plural_deal_yn" value="Y"$param[plural_deal_y]><label for="plural_deal_yn1" class="Column_02">복수거래 중</label>
                                        </td>
                                        <td>
                                            <input type="radio" id="plural_deal_yn2" name="plural_deal_yn" value="N"$param[plural_deal_n]><label for="plural_deal_yn2" class="Column_02">단수거래 중</label>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <!-- 복수거래유무 end -->

                    <!-- 복수거래업체 -->
                    <tr>
                        <th>복수거래업체</th>
                        <td>
                            <table class="basic_detail_table _printBusiness">
                                <tbody>
                                    <tr>
                                        <td>
                                            <select class="basic_input Column_02" id="plural_deal_site_name1" name="plural_deal_site_name1"$param[plural_disabled]>
                                                <option value="">- 선택 -</option>
                                                <option value="애드코아">애드코아</option>
                                                <option value="디티피아">디티피아</option>
                                            </select>
                                            <input type="hidden" id="pre_plural_deal_site_name1" value="$param[plural_deal_site_name1]">
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <input class="basic_input" type="text" id="plural_deal_site_detail1" name="plural_deal_site_detail1" value="$param[plural_deal_site_detail1]" placeholder="세부적으로 적어주세요."$param[plural_disabled]>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <table class="basic_detail_table">
                                <tbody>
                                    <tr>
                                        <td>
                                            <select class="basic_input Column_02" id="plural_deal_site_name2" name="plural_deal_site_name2"$param[plural_disabled]>
                                                <option value="">- 선택 -</option>
                                                <option value="애드코아">애드코아</option>
                                                <option value="디티피아">디티피아</option>
                                            </select>
                                            <input type="hidden" id="pre_plural_deal_site_name2" value="$param[plural_deal_site_name2]">
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <th></th>
                        <td>
                            <input class="basic_input" type="text" id="plural_deal_site_detail2" name="plural_deal_site_detail2" value="$param[plural_deal_site_detail2]" placeholder="세부적으로 적어주세요."$param[plural_disabled]>
                        </td>
                    </tr>
                    <!-- 복수거래업체 end -->

                    <!-- 메모 -->
                    <tr>
                        <th>메모</th>
                        <td>
                            <table class="basic_detail_table">
                                <tbody>
                                    <tr>
                                        <td>
                                            <textarea class="basic_input" style="height:100px; font-size:1.313em !important;" id="memo" name="memo">$param[memo]</textarea>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <!-- 메모 end -->

                </table>
            </div>
            <!-- 성향정보 end -->
            <div class="button_wrapper" style="width:88%;">
                <button class="mypage_bottom_basic_btn_out" type="button">취소</button>
                <button class="mypage_bottom_color_btn_out btn_add_address" type="button">수정완료</button>
            </div>
        </div>

HTML;

    return $html;

}

//선입금 충전 팝업
function prepaymentPop($info) {
    $group_name   = $info["group_name"];
    $member_name  = $info["member_name"];
    $member_mail  = $info["email"];
    $url          = $info["url"];

    $mall_nm    = urlencode("(주)굿프린팅");
    $product_nm = urlencode("선입금 카드충전");
    $user_nm    = $group_name;
    if (!empty($user_nm)) {
        $user_nm = urlencode($user_nm . ' ' . $member_name);
    } else {
        $user_nm = urlencode($member_name);
    }

    $html = <<<HTML
            <header class="popup">
                <h2>선입금 충전하기</h2>
                <button class="close">닫기</button>
            </header>
            <article>
                <h3>거래금액 선택(VAT포함)</h3>
                <ul class="options col6 radio">
                    <li>
                        <label class="radio">
                            <input type="radio" id="price_10" name="charge_price" onclick="changeChargePrice(this);" value="100000" checked>
                            <i><i></i></i>
                            10만원
                        </label>
                    </li>
                    <li>
                        <label class="radio">
                            <input type="radio" id="price_30" name="charge_price" onclick="changeChargePrice(this);" value="300000">
                            <i><i></i></i>
                            30만원
                        </label>
                    </li>
                    <li>
                        <label class="radio">
                            <input type="radio" id="price_50" name="charge_price" onclick="changeChargePrice(this);" value="500000">
                            <i><i></i></i>
                            50만원
                        </label>
                    </li>
                    <li>
                        <label class="radio">
                            <input type="radio" id="price_70" name="charge_price" onclick="changeChargePrice(this);" value="700000">
                            <i><i></i></i>
                            70만원
                        </label>
                    </li>
                    <li>
                        <label class="radio">
                            <input type="radio" id="price_100" name="charge_price" onclick="changeChargePrice(this);" value="1000000">
                            <i><i></i></i>
                            100만원
                        </label>
                    </li>
                    <li>
                        <label class="radio">
                            <input type="radio" id="price_200" name="charge_price" onclick="changeChargePrice(this);" value="2000000">
                            <i><i></i></i>
                            200만원
                        </label>
                    </li>
                    <li>
                        <label class="radio">
                            <input type="radio" id="price_300" name="charge_price" onclick="changeChargePrice(this);" value="3000000">
                            <i><i></i></i>
                            300만원
                        </label>
                    </li>
                    <li>
                        <label class="radio">
                            <input type="radio" id="price_500" name="charge_price" onclick="changeChargePrice(this);" value="5000000">
                            <i><i></i></i>
                            500만원
                        </label>
                    </li>
                </ul>
                <dl class="shadowBox">
                    <dt>총 결제금액 (VAT포함)</dt>
                    <dd class="price"><input id="charge_price" type="text" class="transparent" readonly value="100,000"> 원</dd>
                </dl>

                <!-- PG 결제관련 form -->
                <form name="p_frm_pay" method="post" action="">
                    <!--------------------------->
                    <!-- ::: 공통 인증 요청 값 -->
                    <!--------------------------->

                    <!-- 가맹점 이름 // -->
                    <input type="hidden" id="P_EP_mall_nm"      name="EP_mall_nm"      value="{$mall_nm}">
                    <!-- 가맹점 주문번호 // -->
                    <input type="hidden" id="P_EP_order_no"     name="EP_order_no"     value="">
                    <!-- 통화코드 // 00 : 원화-->
                    <input type="hidden" id="P_EP_currency"     name="EP_currency"     value="00">
                    <!-- 가맹점 CALLBACK URL // -->
                    <input type="hidden" id="P_EP_return_url"   name="EP_return_url"   value="$url/webpay_card_prepay/web/normal/new_order_res.php">
                    <!-- CI LOGO URL // -->
                    <input type="hidden" id="P_EP_ci_url"       name="EP_ci_url"       value="">
                    <!-- 언어 // -->
                    <input type="hidden" id="P_EP_lang_flag"    name="EP_lang_flag"    value="KOR">
                    <!-- 가맹점 CharSet // -->
                    <input type="hidden" id="P_EP_charset"      name="EP_charset"      value="UTF-8">
                    <!-- 사용자구분 // -->
                    <input type="hidden" id="P_EP_user_type"    name="EP_user_type"    value="2">
                    <!-- 가맹점 고객ID // -->
                    <input type="hidden" id="P_EP_user_id"      name="EP_user_id"      value="">
                    <!-- 가맹점 고객일련번호 // -->
                    <input type="hidden" id="P_EP_memb_user_no" name="EP_memb_user_no" value="">
                    <!-- 가맹점 고객명 // -->
                    <input type="hidden" id="P_EP_user_nm"      name="EP_user_nm"      value="{$user_nm}">
                    <!-- 가맹점 고객 E-mail // -->
                    <input type="hidden" id="P_EP_user_mail"    name="EP_user_mail"    value="{$member_mail}">
                    <!-- 가맹점 고객 연락처1 // -->
                    <input type="hidden" id="P_EP_user_phone1"  name="EP_user_phone1"  value="">
                    <!-- 가맹점 고객 연락처2 // -->
                    <input type="hidden" id="P_EP_user_phone2"  name="EP_user_phone2"  value="">
                    <!-- 가맹점 고객 주소 // -->
                    <input type="hidden" id="P_EP_user_addr"    name="EP_user_addr"    value="">
                    <!-- 가맹점 필드1 // -->
                    <input type="hidden" id="P_EP_user_define1" name="EP_user_define1" value="">
                    <!-- 가맹점 필드2 // -->
                    <input type="hidden" id="P_EP_user_define2" name="EP_user_define2" value="">
                    <!-- 가맹점 필드3 // -->
                    <input type="hidden" id="P_EP_user_define3" name="EP_user_define3" value="">
                    <!-- 가맹점 필드4 // -->
                    <input type="hidden" id="P_EP_user_define4" name="EP_user_define4" value="">
                    <!-- 가맹점 필드5 // -->
                    <input type="hidden" id="P_EP_user_define5" name="EP_user_define5" value="">
                    <!-- 가맹점 필드6 // -->
                    <input type="hidden" id="P_EP_user_define6" name="EP_user_define6" value="">
                    <!-- 상품정보구분 // -->
                    <input type="hidden" id="P_EP_product_type" name="EP_product_type" value="">
                    <!-- 서비스 기간 // (YYYYMMDD) -->
                    <input type="hidden" id="P_EP_product_expr" name="EP_product_expr" value="">

                    <!--------------------------->
                    <!-- ::: 카드 인증 요청 값 -->
                    <!--------------------------->

                    <!-- 사용가능한 카드 LIST // FORMAT->카드코드:카드코드: ... :카드코드 EXAMPLE->029:027:031 // 빈값 : DB조회-->
                    <input type="hidden" id="P_EP_usedcard_code"     name="EP_usedcard_code"     value="">
                    <!-- 할부개월 (카드코드-할부개월) -->
                    <input type="hidden" id="P_EP_quota"             name="EP_quota"             value="">
                    <!-- 해외안심클릭 사용여부(변경불가) // -->
                    <input type="hidden" id="P_EP_os_cert_flag"      name="EP_os_cert_flag"      value="2">
                    <!-- 무이자 여부 (Y/N) // -->
                    <input type="hidden" id="P_EP_noinst_flag"       name="EP_noinst_flag"       value="">
                    <!-- 무이자 기간(카드코드-더할할부개월) // -->
                    <input type="hidden" id="P_EP_noinst_term"       name="EP_noinst_term"       value="">
                    <!-- 카드사포인트 사용여부 (Y/N) // -->
                    <input type="hidden" id="P_EP_set_point_card_yn" name="EP_set_point_card_yn" value="">
                    <!-- 포인트카드 LIST  // -->
                    <input type="hidden" id="P_EP_point_card"        name="EP_point_card"        value="">
                    <!-- 조인코드 // -->
                    <input type="hidden" id="P_EP_join_cd"           name="EP_join_cd"           value="">
                    <!-- 국민앱카드 사용유무 // -->
                    <input type="hidden" id="P_EP_kmotion_useyn"     name="EP_kmotion_useyn"     value="">
                    <!-- 가맹점아이디 // -->
                    <!--input type="hidden" id="P_EP_mall_id"           name="EP_mall_id"           value="T0011065"-->
                    <input type="hidden" id="P_EP_mall_id"           name="EP_mall_id"           value="05562983">
                    <!-- 윈도우 타입 // -->
                    <input type="hidden" id="P_EP_window_type"       name="EP_window_type"       value="iframe">
                    <!--  결제수단 // -->
                    <input type="hidden" id="P_EP_pay_type"          name="EP_pay_type"          value="11">
                    <!--  상품명 // -->
                    <input type="hidden" id="P_EP_product_nm"        name="EP_product_nm"        value="{$product_nm}">
                    <!--  상품금액 // -->
                    <input type="hidden" id="P_EP_product_amt"       name="EP_product_amt"       value="">

                    <!--------------------------------->
                    <!-- ::: 인증응답용 인증 요청 값 -->
                    <!--------------------------------->

                    <!--  응답코드 // -->
                    <input type="hidden" id="resCd"         name="resCd"         value="">
                    <!--  응답메세지 // -->
                    <input type="hidden" id="resMsg"        name="resMsg"        value="">
                    <!--  결제창 요청구분 // -->
                    <input type="hidden" id="shopOrderNo"          name="shopOrderNo"          value="">
                    <!--  결제수단 // -->
                    <input type="hidden" id="authorizationId"   name="authorizationId"   value="">
                    <!--  복합결제 여부 (Y/N) // -->
                    <input type="hidden" id="shopValue1" name="shopValue1" value="">
                    <!--  카드코드 (ISP:KVP카드코드 MPI:카드코드) // -->
                    <input type="hidden" id="shopValue2"      name="shopValue2"      value="">
                    <!--  MPI인 경우 ECI코드 // -->
                    <input type="hidden" id="shopValue3"       name="shopValue3"       value="">
                    <!--  거래구분 // -->
                    <input type="hidden" id="shopValue4"  name="shopValue4"  value="">
                    <!--  카드사 세이브 여부 (Y/N) // -->
                    <input type="hidden" id="shopValue5"     name="shopValue5"     value="">
                    <!--  추적번호 // -->
                    <input type="hidden" id="shopValue6"       name="shopValue6"       value="">
                    <!--  세션키 // -->
                    <input type="hidden" id="shopValue7"     name="shopValue7"     value="">
                </form>

                <iframe name="p_iframe_pay" id="p_iframe_pay" width="0" height="0" style="display:none;"></iframe>
                <!--ul class="guide transparent">
                    <li>농협이나 씨티카드로 결제하시는 경우,<br>결제 후 02-2260-9000(고객센터)로 연락바랍니다.</li>
                </ul-->
                <hr class="end">
                <footer class="action">
                    <button type="button" class="exe" onclick="doCharge();">결제하기</button>
                    <button type="button" class="close">취소</button>
                </footer>
            </article>

HTML;

    return $html;
}
?>
