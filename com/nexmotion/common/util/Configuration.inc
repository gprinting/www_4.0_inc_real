<?
class Configuration {
    public $templatePath;

    function __construct() {
        $this->templatePath = $_SERVER["DOCUMENT_ROOT"] . "/design_template/";
    }
}
?>
