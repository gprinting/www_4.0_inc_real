<?php
class NimdaCommonHtmlUtil {
    private $conn;
    private $dao;

    function __construct($param = null) {
        if (!empty($param)) {
            $this->init($param);
        }
    }
    function __destruct() {
        $this->conn->Close();
    }

    /**
     * @brief 클래스 필드 초기화
     *
     * @param $param = 필드 초기화용 변수배열
     */
    public function init($param) {
        $this->conn = $param["conn"];
        $this->dao  = $param["dao"];
    }

    /**
     * @brief 셀렉트박스 option html 생성
     *
     * @param $text = 표시값
     * @param $val  = 실제값
     * @param $attr = 속성값
     *
     * @return option html
     */
    private function makeOption($text, $val, $attr = '') {
        return sprintf("<option %s value=\"%s\">%s</option>", $attr
                                                            , $val
                                                            , $text);
    }

    /**
     * @brief 판매채널 html 생성
     *
     * @return select box, option html
     */
    public function getSellSiteHtml() {
        $ret = '';

        $ret .= $this->makeOption("GP", "GP");
        $ret .= $this->makeOption("DP", "DP");

        return $ret;
    }
    
    /**
     * @brief 회원등급 html 생성
     *
     * @return select box, option html
     */
    public function getMemberGradeHtml() {
        $rs = $this->dao->selectMemberGradePolicy($this->conn);
        $ret = '';
        while ($rs && !$rs->EOF) {
            $fields = $rs->fields;

            $grade_name = $fields["erp_grade_name"] . '(' . $fields["grade"] . ')';
            $ret .= option($grade_name, $fields["grade"]);

            $rs->MoveNext();
        }

        return $ret;
    }
}
