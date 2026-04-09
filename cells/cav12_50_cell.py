from .wt_cell import DGGranuleLikeCell, CAV12_50_BK_SPLIT


class Cav12_50Cell(DGGranuleLikeCell):
    def __init__(self, name="cav12_50_cell"):
        super().__init__(name=name, bk_split=CAV12_50_BK_SPLIT)
        self.scale_cav12(0.5)


Cell50 = Cav12_50Cell