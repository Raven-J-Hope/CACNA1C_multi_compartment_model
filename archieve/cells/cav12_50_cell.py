from .wt_cell import DGGranuleLikeCell, CAV12_50_BK_SPLIT


class Cav12_50Cell(DGGranuleLikeCell):
    def __init__(self, name="cav12_50_cell", bk_split=None, channel_overrides=None):
        super().__init__(
            name=name,
            bk_split=CAV12_50_BK_SPLIT if bk_split is None else bk_split,
            channel_overrides=channel_overrides,
        )
        self.scale_cav12(0.5)