<?php
include_once(INC_PATH  . "/common_dao/ProductCommonDAO.inc");

class GuideDAO extends ProductCommonDAO {
    /**
     * @brief 이용안내 재질설명 검색
     *
     * @param $conn  = db connection
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    public function selectPaperDscr($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* GuideDAO.inc - 재질설명 검색 */";
        $query .= "\n         A.name";
        $query .= "\n        ,A.dvs";
        $query .= "\n        ,A.color";
        $query .= "\n        ,A.basisweight";
        $query .= "\n        ,A.dscr";
        $query .= "\n        ,A.purp";
        $query .= "\n   FROM  paper_dscr AS A";
        $query .= "\n  WHERE  A.top = %s";
        $query .= "\n  ORDER BY name, dvs, color";

        $query  = sprintf($query, $conn->param($param["top"]));

        $bind = [];
        $bind[] = $param["top"];

        $rs = $conn->Execute($query, $bind);

        return $rs;
    }
}
