erDiagram
    ユーザー {
        uuid id PK "ユーザーのユニークな識別子"
        text 名前 "ユーザーの名前"
        text メールアドレス "ユーザーの連絡先メールアドレス"
        timestamp_with_time_zone 作成日時 "レコードが作成された日時"
    }
    会議室 {
        uuid id PK "会議室のユニークな識別子"
        text 部屋名 "会議室の名前または番号"
        integer 収容人数 "会議室の最大収容人数"
        text 説明 "会議室に関する追加の説明"
        timestamp_with_time_zone 作成日時 "レコードが作成された日時"
    }
    予約 {
        uuid id PK "予約のユニークな識別子"
        uuid ユーザーID FK "予約を行ったユーザーのID"
        uuid 会議室ID FK "予約された会議室のID"
        timestamp_with_time_zone 開始日時 "予約の開始日時"
        timestamp_with_time_zone 終了日時 "予約の終了日時"
        text ステータス "予約のステータス"
        timestamp_with_time_zone 作成日時 "レコードが作成された日時"
    }
    
    予約 }|--|| ユーザー : "予約者"
    予約 }|--|| 会議室 : "予約対象"