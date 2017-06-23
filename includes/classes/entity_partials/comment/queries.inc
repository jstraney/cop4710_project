<?php

$create_query = <<<EOT
CALL comment_on_event(:content, :event_id, :user_id, :uni_id);
EOT;

// once a comment is created it implies that the user could create
// a comment on the event. user id is passed here from the session,
// so additional checks won't be necessary
$update_query = <<<EOT
UPDATE commented_on SET
content = :content
WHERE comment_id = :comment_id
AND user_id = :user_id
EOT;

$destroy_query = <<<EOT
DELETE FROM commented_on
WHERE comment_id = :comment_id
AND user_id = :user_id;
EOT;

// get all comments on an event. used in the 'comments/event' api endpoint
$comments_event_query = <<<EOT
SELECT c.comment_id, c.content, c.user_id, u.user_name, c.date_posted
FROM commented_on c, users u, events e
WHERE e.event_id = :event_id
AND u.user_id = c.user_id
AND c.event_id = e.event_id
ORDER BY c.date_posted
LIMIT :start, :end;
EOT;
?>
