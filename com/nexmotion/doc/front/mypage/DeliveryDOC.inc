<?
function getAddressMain($param) {

    $html = <<<HTML

    <section class="searchCondition">
        <!--
        <div class="rowWrap">
            <dl class="date">
                <dt>등록일</dt>
                <dd>
                    <input type="text" class="_date" id="from" name="from" value="">
                    ~
                    <input type="text" class="_date" id="to" name="to" value="">
                    <ul class="preset">
                        <li><button onclick="dateSet(0); return false;">오늘</button></li>
                        <li><button onclick="dateSet(1); return false;">어제</button></li>
                        <li><button onclick="dateSet(7); return false;">일주일</button></li>
                        <li><button onclick="dateSet(30); return false;">한달</button></li>
                        <li><button onclick="dateSet('all'); return false;">전체</button></li>
                    </ul>
                </dd>
            </dl>
        </div>
        -->
        <div class="rowWrap">
            <dl class="delivery full">
                <dt>검색조건</dt>
                <dd>
                    <select id="category" name="category">
                        <option value="dlvr_name">업체/상호명</option>
                        <option value="recei">담당자</option>
                        <option value="tel_num">연락처</option>
                        <option value="cell_num">휴대전화</option>
                    </select>
                </dd>
                <dd class="rest">
                    <input type="text" id="searchkey" name="searchkey">
                </dd>
            </dl>
        </div>
        <button onclick="searchDlvrList();" class="search">검색</button>
    </section>
    <form name="frm" id="frm">
    <table class="list info searchResult">
        <caption>
            <div class="resultNum" id="resultNum">
                $param[html]
            </div>
            <select class="listNum" onchange="changeListNum(this.value);">
                <option value="10">10개씩 보기</option>
                <option value="20">20개씩 보기</option>
                <option value="50">50개씩 보기</option>
            </select>
        </caption>
        <colgroup>
            <col class="check" style="width:50px;">
            <col class="date" style="width:100px;">
            <col class="corp" style="width:100px;">
            <col class="name" style="width:100px;">
            <col>
            <col class="tel" style="width:120px;">
            <col class="btn2" style="width:100px;">
        </colgroup>
        <thead>
            <tr>
                <th><label class="check"><input type="checkbox" class="_general" onclick="allCheck();"><i><i></i></i></label></th>
                <th>등록일</th>
                <th>업체/상호명</th>
                <th >담당자</th>
                <th>주소</th>
                <th>연락처</th>
                <th>관리</th>
            </tr>
        </thead>
        <tbody id="list">
            $param[list]
        </tbody>
    </table>
    </form>
     <ul class="paging" id="paging">
            $param[paging]
        </ul>
    <hr class="end">
    <div class="end">
        <div class="function withEnd" style="margin-top:0;">
            <button class="c_btn-point-reverse remove" onclick="multiDel(); return false;">선택 배송지 삭제</button>
        </div>
        <div class="left">
            <button type="button" class="progress" onclick="layerPopup('l_address', 'popup/l_address_regi.html')">나의 배송지 추가</button>
            <button type="button" onclick="dlvrExcelDown();">배송지 csv 다운로드</button>
            <button type="button" onclick="showExcelUploadPopup();">배송지 csv 업로드</button>
            <!--<iframe id="file_ifr" width="0" height="0" style="display:none;" src=""></iframe>-->
        </div>
    </div>
    <footer class="action">
        <!--<div class="left">
            <button type="button" class="progress" onclick="layerPopup('l_address', 'popup/l_address_regi.html')">나의 배송지 추가</button>
            <button type="button" onclick="dlvrExcelDown();">배송지 csv 다운로드</button>
            <button type="button" onclick="showExcelUploadPopup();">배송지 csv 업로드</button>
            <iframe id="file_ifr" width="0" height="0" style="display:none;" src=""></iframe>
        </div>-->
        <!--<ul class="paging" id="paging">
            $param[paging]
        </ul>-->
    </footer>
</section>

HTML;

    echo $html;
}

function getAddressTypeMain() {

    $html = <<<HTML

    <h4 class="regularDelivery">월배송 안내</h4>
    <table class="view">
        <colgroup>
            <col class="th">
            <col>
            <col class="th">
            <col>
            <col class="action">
        </colgroup>
        <tbody>
            <tr>
                <th>배송요금</th>
                <td colspan="3"><strong class="number">55,000</strong> 원 (부가세 포함), 선입금 환불 불가</td>
                <td rowspan="6" class="action final">
                    <a class="exe" onclick="location.href='/mypage22/add_dlvr.html'">서비스 신청하기</a>
                </td>
            </tr>
            <tr>
                <th>배송기간</th>
                <td>매월 1일부터 말일까지</td>
                <th>신청기간</th>
                <td>매월 말일부터 익월5일까지</td>
            </tr>
            <tr>
                <th rowspan="4">이용조건</th>
                <td colspan="3">1. 신규 가입 고객: 담당자 통화 후 가입 해당월만 서비스 지원.</td>
            </tr>
            <tr>
                <td colspan="3">2. 전월 매출액 기준: 33만원(부가세 포함) 이상 -> 무료배송</td>
            </tr>
            <tr>
                <td colspan="3">3. 전월 매출액 기준: 33만원(부가세 포함) 미만 -> 유료배송</td>
            </tr>
            <tr>
                <td colspan="3">4. 월배송(직배송)비: 55,000원(부가세 포함) 선입금 신청 후 이용</td>
            </tr>
        </tbody>
    </table>
    <article class="whiteWrap">
        <div class="col trucks">
            <h5>호차별 배송지역 : 낮배송(오후1시경 출고)</h5>
            <dl>
                <dt>A-1호차</dt>
                <dd>광진구, 강동구, 송파구 일부</dd>
            </dl>
            <dl>
                <dt>A-2호차</dt>
                <dd>덕양구, 일산, 금촌</dd>
            </dl>
            <dl>
                <dt>A-3호차</dt>
                <dd>마포구 일부, 영등포구</dd>
            </dl>
            <dl>
                <dt>A-4호차</dt>
                <dd>양천구, 강서구 일부</dd>
            </dl>
            <dl>
                <dt>A-5호차</dt>
                <dd>서대문, 광화문, 마포일부, 은평구</dd>
            </dl>
            <dl>
                <dt>A-6호차</dt>
                <dd>광진구일부, 송파구일부, 성남, 분당</dd>
            </dl>
            <dl>
                <dt>A-7호차</dt>
                <dd>의정부, 도봉구일부</dd>
            </dl>
            <dl>
                <dt>A-8호차</dt>
                <dd>의정부일부, 포천, 양주, 동두천, 백석</dd>
            </dl>
            <dl>
                <dt>A-9호차</dt>
                <dd>미아동, 수유동, 창동, 노원, 중랑구, 구리</dd>
            </dl>
            <dl>
                <dt>A-10호차</dt>
                <dd>용산, 구로, 금천, 방배동</dd>
            </dl>
            <dl>
                <dt>A-11호차</dt>
                <dd>안산, 시흥일부</dd>
            </dl>
            <dl>
                <dt>A-12호차</dt>
                <dd>강서구일부, 부천, 시흥일부</dd>
            </dl>
            <dl>
                <dt>A-13호차</dt>
                <dd>인천, 부평, 계양</dd>
            </dl>
            <h5>호차별 배송지역 : 밤배송(저녁8시경 출고)</h5>
            <dl>
                <dt>B-1호차</dt>
                <dd>인천, 부평, 계양</dd>
            </dl>
            <dl>
                <dt>B-2호차</dt>
                <dd>수원일부, 화성, 발안</dd>
            </dl>
            <dl>
                <dt>B-3호차</dt>
                <dd>안양일부, 부천, 군포, 광명, 검단, 김포,화성</dd>
            </dl>
            <dl>
                <dt>B-4호차</dt>
                <dd>광주, 이천, 용인, 죽전, 수원일부</dd>
            </dl>
            <dl>
                <dt>B-5호차</dt>
                <dd>병점, 오산, 평택, 천안, 온양</dd>
            </dl>
            <dl>
                <dt>B-6호차</dt>
                <dd>하남, 마석, 춘천, 원주, 여주, 양평</dd>
            </dl>
        </div>
        <div class="col">
            <h5>월배송 상세 안내</h5>
            <ul class="guide">
                <li>월배송(직배송) 이용료는 고객님의 신청 후 선입금 계좌에서 결제됩니다.</li>
                <li>신청 승인이 완료되어 차감 결제된 월배송(직배송) 이용료는 돌려드리지 않습니다.</li>
                <li>월배송(직배송) 이용료 결제 조회는 결제정보 입출금 내역에서 조회가 가능합니다.</li>
                <li>물량이 많은 경우는 분할되어 2회에 걸쳐 배송될 수 있습니다.</li>
                <li>누락분 발생 경우에도 2회에 걸쳐 배송될 수 있으며 다른 배송은 불가합니다.</li>
                <li>배송지역에 따라 출고 시간이 낮배송(오후1시경), 저녁배송(저녁8시경) 나누어지며,배송지역 별 배송차량과 출고시간을 확인하시길 바랍니다.</li>
                <li>배송지역에 따라 도착 시간의 차이가 크며, 저녁배송은 새벽까지 배송이 되기 때문에 안전하게 배송을 받으실 수 있는 장소가 중요합니다. 정해진 곳에 배송 후 분실 시 책임지지 않습니다.</li>
                <li>배송시간은 날씨 및 도로교통 상황에 따라 변동 될 수 있습니다.</li>
                <li>일요일, 공휴일은 배송되지 않습니다.</li>
                <li>신규 신청당일은 입금 확인이 되어도 배송준비로 배송이 안될 수도 있습니다.</li>
            </ul>
            <h5>배송지역 확인 문의</h5>
            <dl class="contact">
                <dt>황재호 배송과장</dt>
                <dd>010-3841-1284</dd>
            </dl>
        </div>
    </article>
    <hr class="end">
    <h4 class="rapidDelivery">선물택배 안내</h4>
    <article class="whiteWrap">
        <ul class="guide">
            <li>아래의 내용으로 주문된 상품건만 택배 선불처리가 가능합니다.</li>
            <li>선불 택배는 주문 시 요청하신 건에 한하여 서비스하고 있습니다.</li>
            <li>선불 택배 요청 시 <strong>주문금액과 선불 택배비의 금액이 선입</strong>되어 있어야 선불 택배 처리가 가능합니다.</li>
            <li>만약 <strong>선입금 금액이 부족한 경우에는 부득이 착불로 처리</strong>됨을 알려드립니다.</li>
            <li>명함과 사이즈가 비슷한 복권, 스티커는 묶음이 가능합니다.</li>
            <li>선불 택배는 기본적으로 선불택배 가능상품으로만 함께 주문되어야하며, 배송지가 한곳으로 동일해야만 가능합니다.</li>
            <li><strong>묶음배송</strong>은 기존과는 달리 장바구니에서 <strong>상품들을 한 번에 주문</strong>하셔야 묶음배송으로 처리됩니다. (상품들의 배송지가 같을 경우에만 가능)</li>
            <li>아래에 기재되어있지 않은 상품들을 선불 택배 요청 시에는 고객센터로 문의하시기 바랍니다.</li>
        </ul>
        <h5>선불택배 가격정보</h5>
        <table class="view info">
            <colgroup>
                <col>
                <col>
                <col>
                <col>
                <col>
                <col>
                <col>
                <col>
                <col>
                <col>
                <col>
            </colgroup>
            <thead>
                <tr>
                    <th rowspan="2" class="lastTr"></th>
                    <th rowspan="2" class="lastTr">명함</th>
                    <th rowspan="2" class="lastTr">복권</th>
                    <th rowspan="2" class="lastTr">스티커</th>
                    <th colspan="4" class="narrow">전단</th>
                    <th colspan="3" class="narrow">봉투</th>
                </tr>
                <tr>
                    <th class="sub noFirst">90g 아트 16절<br>(46계열)</th>
                    <th class="sub">80g 모조 16절<br>(46계열)</th>
                    <th class="sub">80g 모조 A4<br>(국전계열)</th>
                    <th class="sub">90g 아트 A4<br>(국전계열)</th>
                    <th class="sub">소봉투<br>(120g 이하)</th>
                    <th class="sub">대봉투<br>(120g 이하)</th>
                    <th class="sub">대봉투<br>(150g 이하)</th>
            </thead>
            <tbody>
                <tr>
                    <th>수량</th>
                    <td>10,000</td>
                    <td>10,000</td>
                    <td>10,000</td>
                    <td>8,000</td>
                    <td>8,000</td>
                    <td>4,000</td>
                    <td>4,000</td>
                    <td>1,000</td>
                    <td>1,000</td>
                    <td>1,000</td>
                </tr>
                <tr>
                    <th scope="row">기준</th>
                    <td>명함 사이즈</td>
                    <td>명함 사이즈</td>
                    <td>명함 사이즈</td>
                    <td>1연</td>
                    <td>1연</td>
                    <td>1연</td>
                    <td>1연</td>
                    <td>120g 이하</td>
                    <td>120g 이하</td>
                    <td>150g 이하</td>
                </tr>
                <tr>
                    <th scope="row">기본비</th>
                    <td><span class="number">2,750</span>원</td>
                    <td><span class="number">2,750</span>원</td>
                    <td><span class="number">2,750</span>원</td>
                    <td><span class="number">6,600</span>원</td>
                    <td><span class="number">6,600</span>원</td>
                    <td><span class="number">3,300</span>원</td>
                    <td><span class="number">3,300</span>원</td>
                    <td><span class="number">2,750</span>원</td>
                    <td><span class="number">3,300</span>원</td>
                    <td><span class="number">6,050</span>원</td>
                </tr>
            </tbody>
        </table>
    </article>
    <hr class="end">
    <h4 class="variousDelivery">기타 배송방법 안내</h4>
    <article class="whiteWrap">
        <dl>
            <dt>착불택배</dt>
            <dd>
                <p class="guide arrow">택배를 선택하시면, CJ택배로 배송됩니다. 배송 금액은 착불입니다.</p>
                <dl class="contact office">
                    <dt>CJ택배 대표번호</dt>
                    <dd>1588-1255</dd>
                </dl>
                <a href="https://www.doortodoor.co.kr/" target="_blank" class="cubic vivid">웹사이트 바로가기</a>
            </dd>
        </dl>
        <dl>
            <dt>퀵서비스</dt>
            <dd>
                <p class="guide arrow">오토바이 퀵배송을 뜻하며, 굿프린팅은 지정된 업체를 통해 배송합니다. 짐콜(오토바이택배)</p>
                <dl class="contact office">
                    <dt>짐콜 대표번호</dt>
                    <dd>1544-3165</dd>
                </dl>
                <button type="button" class="cubic vivid" onclick="window.open('/design_template/mypage22/popup/pop_ order_DeliQ.php','','scrollbars=yes,width=500,height=600,top=300,left=550')">요금 보기</button>
            </dd>
        </dl>
        <dl>
            <dt>방문수령</dt>
            <dd>
                <dl>
                    <dt>➀ 본사</dt>
                    <dd>
                        <p class="guide arrow">서울시 중구 필동2가 84-12번지 우리빌딩 2층</p>
                        <a href="http://yesprinting.co.kr/cscenter22/contact_map.html" target="_blank" class="cubic vivid">약도 보기</a>
                    </dd>
                </dl>
            </dd>
            <dd>
                <dl>
                    <dt>➁ 인현동</dt>
                    <dd>
                        <p class="guide arrow">서울시 중구 충무로4가 120-3번지 제 1층 9, 10호 디지털프린팅<br>(구주소 : 서울시 중구 인현동 2가 134-4 1층 디지털 프린팅)</p>
                        <a href="http://yesprinting.co.kr/cscenter22/contact_map.html" target="_blank" class="cubic vivid">약도 보기</a>
                    </dd>
                </dl>
            </dd>
        </dl>
        <dl>
            <dt>용달</dt>
            <dd>
                <p class="guide arrow">용달은 다마스와 1톤을 선택하실 수 있습니다.</p>
                <dl class="contact office">
                    <dt>짐콜 대표번호</dt>
                    <dd>1544-3165</dd>
                </dl>
                <button tyoe="button" class="cubic vivid" onclick="window.open('/design_template/mypage22/popup/pop_ order_DeliDA.php','','scrollbars=no,width=540,height=320,top=300,left=550')">요금 보기</button>
            </dd>
        </dl>
        <dl>
            <dt>화물</dt>
            <dd>
                <p class="guide arrow">화물은 대신화물을 이용하실 수 있으며,<br>필히 영업소를 적어주셔야합니다.<br>대신화물 사이트를 통해 전국 영업소와 가격표를 보실 수 있습니다.</p>
                <dl class="contact office">
                    <dt>짐콜 대표번호</dt>
                    <dd>043-222-4582</dd>
                </dl>
                <a href="http://apps.ds3211.co.kr/" target="_blank" class="cubic vivid">웹사이트 바로가기</a>
                </span>
            </dd>
        </dl>
    </article>


HTML;

    echo $html;
}
?>
