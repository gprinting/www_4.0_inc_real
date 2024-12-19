<?
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . '/com/nexmotion/common/util/nimda/ExcelLib.inc');

class PopSpcExcelUtil extends ExcelLib {
    /**
     * @brief 사용자가 검색한 조건으로 엑셀시트를 생성한다.
     *
     * @detail 이 파일을 사용할지 말지 불투명하다.
     *
     * @param $sheet_name       = 엑셀파일 워크시트명
     */
    function makePopSpcExcelSheet($sheet_name
                                 ) {
        echo $sheet_name;
        //정보를 입력한 워크시트 생성
        $sheet_name = strval($sheet_name);
        $this->active_sheet = $this->createSheet($sheet_name);

        

    }        


}

?>
