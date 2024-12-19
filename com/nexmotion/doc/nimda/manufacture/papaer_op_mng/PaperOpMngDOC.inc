<?
//종이발주 상세
function getPaperOpDetail($param) {

    $html = <<<HTML
    <div class="modal_title">
        <span class="modal_title_span">종이발주</span>
    </div>
    <a class="close_popup" onclick="hideRegiPopup();">x</a>

	<div class="pop-base">
		<div class="pop-content ofa" style="padding:0;">
        <div class="table-body3" style="padding-bottom:0;">
        <div class="table_basic none-hover">
            <table class="table" style="width:100%;">
                 <tbody>
                     <tr>
                         <th>조판번호</th>
                         <td>$param[typset_num]</td>
                         <th>출력명</th>
                         <td>$param[output_name]</td>
                     </tr>
                 </tbody>
            </table>
         </div>
	     </div> <!-- pop-content -->
	</div>
HTML;

    return $html;
}


